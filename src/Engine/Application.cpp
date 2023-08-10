#include "EngineCore/Application.h"
#include "EngineCore/Window.h"

#include "EngineCore/System/Log.h"
#include "EngineCore/Resources/ResourceManager.h"

#include "../Examples/TanksDemo/TanksDemoGame.h"
#include "../Examples/SnakeDemo/SnakeDemoGame.h"
#include "../Examples/PongDemo/PongDemoGame.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <chrono>

Application::Application()
{
    LOG_INFO("Starting Application");
}

Application::~Application()
{
    LOG_INFO("Closing Application");
    ResourceManager::unloadAllResources();
}

int Application::start(glm::ivec2& window_size, const char* title)
{   
    m_pCloseWindow = false;
    m_pWindow = std::make_unique<Window>(title, window_size);
    m_event_dispather.add_event_listener<EventWindowResize>([](EventWindowResize& e)
        {
            LOG_INFO("[EVENT] Resize: {0}x{1}", e.width, e.height);
            RenderEngine::Renderer::setViewport(e.width, e.height);
        });
    m_event_dispather.add_event_listener<EventMouseMoved>([](EventMouseMoved& e)
        {
            LOG_INFO("[EVENT] Mouse moved to {0}x{1}", e.x, e.y);
        });
    m_event_dispather.add_event_listener<EventWindowClose>([&](EventWindowClose& e)
        {
            LOG_INFO("[EVENT] Window close");
            m_pCloseWindow = true;
        });
    m_pWindow->set_event_callback(
        [&](BaseEvent& e)
        {
            m_event_dispather.dispatch(e);
        });

    ResourceManager::loadJSONresources("res/resources.json");

    if (!init())
    {
        return -1;
    }

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!m_pCloseWindow)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
        lastTime = currentTime;

        m_pWindow->on_update();
        on_update(duration);
    }

    m_pWindow = nullptr;

    return 0;
}
