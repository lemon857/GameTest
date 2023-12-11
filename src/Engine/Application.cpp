#include "EngineCore/Engine/Application.h"
#include "EngineCore/Engine/Window.h"
#include "EngineCore/System/Log.h"
#include "EngineCore/System/Stopwatch.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Sound/SoundEngine.h"

Application::Application()
{
    LogSystem::open_log_file("lastest.log");
    LOG_INFO("Starting Application");
}

Application::~Application()
{
    LOG_INFO("Closing Application");
    ResourceManager::unloadAllResources();
    SoundEngine::uninit_audio();
}

int Application::start(glm::ivec2& window_size, const char* title, const char* json_rel_path, const char* ini_rel_path)
{
    m_watch = new Stopwatch();

    INIdata data{ window_size, m_window_position, m_maximized_window };
    ResourceManager::load_INI_settings(ini_rel_path, data, false);

    m_pCloseWindow = false;
    m_pWindow = std::make_unique<Window>(title, m_window_position, window_size, m_maximized_window);

    if (SoundEngine::init_audio() != 0) LOG_ERROR("Fail init sound engine");

    ResourceManager::load_JSON_resources(json_rel_path);

    if (!init_events())
    {        
        return -1;
    }
    if (!init())
    {
        return -1;
    }
   
    LOG_INFO("Renderer: {0}", RenderEngine::Renderer::getRendererStr());
    LOG_INFO("OpenGL version: {0}", RenderEngine::Renderer::getVersionStr());
    
    LOG_INFO("Time initialization: {0}", m_watch->stop());

    delete m_watch;

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!m_pCloseWindow)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        on_update(duration);

        on_ui_render();        

        m_pWindow->on_update();
        on_key_update(duration);
    }

    INIdata endData{ m_pWindow->get_size(), m_window_position, m_maximized_window};
    ResourceManager::load_INI_settings(ini_rel_path, endData, true);
    m_pWindow = nullptr;

    return 0;
}