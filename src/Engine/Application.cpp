#include "EngineCore/Application.h"
#include "EngineCore/Window.h"

#include "EngineCore/System/Log.h"
#include "EngineCore/Resources/ResourceManager.h"

#include "../Examples/TanksDemo/TanksDemoGame.h"
#include "../Examples/SnakeDemo/SnakeDemoGame.h"
#include "../Examples/PongDemo/PongDemoGame.h"

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
    m_pWindow = std::make_unique<Window>(title, window_size);

    ResourceManager::loadJSONresources("res/resources.json");

    if (!init())
    {
        return -1;
    }

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (true)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
        lastTime = currentTime;

        m_pWindow->on_update();
        on_update(duration);
    }

    return 0;
}
