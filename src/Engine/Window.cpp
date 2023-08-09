#include "EngineCore/Window.h"

#include "EngineCore/System/Log.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Physics/PhysicsEngine.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

//typedef ResizeWindowCallback;

Window::Window(std::string title, glm::ivec2& window_size)
	: m_title(std::move(title))
	, m_pWindow(nullptr)
	, m_window_size(window_size)
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

int Window::init()
{
    LOG_INFO("Creating window {0} size {1}x{2}", m_title, m_window_size.x, m_window_size.y);
    if (!glfwInit())
    {
        LOG_CRIT("GLFW init failed");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_pWindow = glfwCreateWindow(m_window_size.x, m_window_size.y,
        m_title.c_str(), nullptr, nullptr);

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
