#include "EngineCore/Window.h"

#include "EngineCore/System/Log.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Physics/PhysicsEngine.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

Window::Window(std::string title, glm::ivec2& window_size)
    : m_data({ std::move(title), window_size, nullptr })
{
	init();
}

Window::~Window()
{
	shuitdown();
}

void Window::on_update()
{
    glfwSwapBuffers(m_pWindow);
    glfwPollEvents();    
}

void Window::set_event_callback(const EventCallback& callback)
{
    m_data.event_callback = callback;
}

int Window::init()
{
    LOG_INFO("Creating window {0} size {1}x{2}", m_data.title, m_data.window_size.x, m_data.window_size.y);
    if (!glfwInit())
    {
        LOG_CRIT("GLFW init failed");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_pWindow = glfwCreateWindow(m_data.window_size.x, m_data.window_size.y,
        m_data.title.c_str(), nullptr, nullptr);

    if (!m_pWindow)
    {
        LOG_CRIT("Generate window failed");
        shuitdown();
        return -1;
    }

    glfwMakeContextCurrent(m_pWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_CRIT("Glad load failed");
        return -1;
    }

    glfwSetWindowUserPointer(m_pWindow, &m_data);

    glfwSetWindowSizeCallback(m_pWindow, 
        [](GLFWwindow* pWindow, int width, int height)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            data.window_size = glm::ivec2(width, height);
            EventWindowResize e(width, height);
            data.event_callback(e);
        });

    glfwSetCursorPosCallback(m_pWindow,
        [](GLFWwindow* pWindow, double x, double y)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

            EventMouseMoved e(x, y);
            data.event_callback(e);
        });

    glfwSetWindowCloseCallback(m_pWindow,
        [](GLFWwindow* pWindow)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

            EventWindowClose e = EventWindowClose();
            data.event_callback(e);
        });

    RenderEngine::Renderer::setClearColor(0, 0, 0, 1);
    RenderEngine::Renderer::setDepthTest(true);

    Physics::PhysicsEngine::init();

	return 0;
}

void Window::shuitdown()
{
    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
}
