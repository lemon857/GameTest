#include "EngineCore/Engine/Application.h"
#include "EngineCore/Engine/Window.h"
#include "EngineCore/System/Log.h"
#include "EngineCore/System/Stopwatch.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Sound/SoundEngine.h"

Application::Application()
{}

Application::~Application()
{}

int Application::start(glm::ivec2& window_size, const char* title, const char* json_rel_path, const char* ini_rel_path, const char* icon_rel_path, double tps_max)
{
    LogSystem::init_log_system("logs");
    //LOG_INFO("Build name: {0}", BUILD_NAME);
    LOG_INFO("Starting Application");

    m_watch = new Stopwatch();

    m_window_size = window_size;

    INIregionSTARTUP startup{ &m_window_size, &m_window_position, &m_maximized_window, &m_fullscreen_window };

    m_ini_data.add_region("STARTUP", &startup);

    ResourceManager::load_INI_settings(ini_rel_path, m_ini_data, false);

    m_pCloseWindow = false;
    m_pWindow = std::make_unique<Window>(title, ResourceManager::getExeFilePath() + icon_rel_path, m_window_position, m_window_size, m_maximized_window, m_fullscreen_window);

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
    LOG_INFO("==========================================");
    
    delete m_watch;

    m_current_tps = 0;
    m_current_fps = 0;
    m_time_tick = 0;
    m_ticks = 0;
    m_time_frame = 0;
    m_frames = 0;

    auto lastTime = std::chrono::high_resolution_clock::now();      

    double sum_time = 0;

    m_max_time_tps = 1000.0 / (tps_max + 1);

    while (!m_pCloseWindow)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
        lastTime = currentTime;

        sum_time += duration;

        if (sum_time >= m_max_time_tps)
        {
#ifdef DEBUG
            on_update(duration);
#else
            on_update(sum_time);
#endif
            // tps counter
            if (m_ticks < 5)
            {
                m_ticks++;
                m_time_tick += sum_time;
            }
            else
            {
                m_current_tps = int((m_ticks / m_time_tick) * 1000.f);
                m_ticks = 0;
                m_time_tick = 0;
            }
            sum_time = 0;
        }

        on_render();

        on_ui_render();
        on_key_update(duration);

        m_pWindow->on_update();    

        // fps counter
        if (m_frames < 5)
        {
            m_frames++;
            m_time_frame += duration;
        }
        else
        {
            m_current_fps = int((m_frames / m_time_frame) * 1000.f);
            m_frames = 0;
            m_time_frame = 0;
        }
    }

    m_fullscreen_window = m_pWindow->is_fullscreen();
    m_window_size = m_pWindow->get_size();

    ResourceManager::load_INI_settings(ini_rel_path, m_ini_data, true);

    m_pWindow = nullptr;

    return 0;
}

void Application::stop()
{    
    LOG_INFO("Closing Application");
    terminate();
    ResourceManager::unloadAllResources();
    SoundEngine::uninit_audio();
    LogSystem::uninit_log_system();
}

void Application::set_max_tps(double max)
{
    m_max_time_tps = 1000.0 / (max + 1);
}

void INIregionSTARTUP::parse(std::string name, std::string value)
{
    if (name == "window_width") window_size->x = std::stoi(value);
    else if (name == "window_height") window_size->y = std::stoi(value);
    else if (name == "window_position_x") window_position->x = std::stoi(value);
    else if (name == "window_position_y") window_position->y = std::stoi(value);
    else if (name == "window_maximized") *maximized_window = value == "true";
    else if (name == "window_fullscreen") *fullscreen = value == "true";
}

std::string INIregionSTARTUP::get_str_data()
{
    std::string data = "";
    data += "window_width=" + std::to_string(window_size->x) + "\n";
    data += "window_height=" + std::to_string(window_size->y) + "\n";
    data += "window_position_x=" + std::to_string(window_position->x) + "\n";
    data += "window_position_y=" + std::to_string(window_position->y) + "\n";
    data += "window_maximized=" + std::to_string(*maximized_window == true ? "true" : "false") + "\n";
    data += "window_fullscreen=" + std::to_string(*fullscreen == true ? "true" : "false") + "\n";
    return data;
}
