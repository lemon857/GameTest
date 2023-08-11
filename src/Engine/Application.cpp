#include "EngineCore/Application.h"
#include "EngineCore/Window.h"
#include "EngineCore/Input.h"

#include "EngineCore/System/Log.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <chrono>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

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
    m_cam = new Camera(glm::vec3(0));

    m_pCloseWindow = false;
    m_pWindow = std::make_unique<Window>(title, window_size);
    m_event_dispather.add_event_listener<EventWindowResize>([](EventWindowResize& e)
        {
            LOG_INFO("[EVENT] Resize: {0}x{1}", e.width, e.height);
            RenderEngine::Renderer::setViewport(e.width, e.height);
        }); 
    m_event_dispather.add_event_listener<EventKeyPressed>([](EventKeyPressed& e)
            {
                if (e.key_code <= KeyCode::KEY_Z)
                {
                    if (e.repeated)
                    {
                        LOG_INFO("[EVENT] Key repeated {0}", static_cast<char>(e.key_code));
                    }
                    else
                    {
                        LOG_INFO("[EVENT] Key pressed {0}", static_cast<char>(e.key_code));
                    }
                }
                Input::pressKey(e.key_code);
            });
    m_event_dispather.add_event_listener<EventKeyReleased>([](EventKeyReleased& e)
                {
                if (e.key_code <= KeyCode::KEY_Z)  LOG_INFO("[EVENT] Key released {0}", static_cast<char>(e.key_code));
                Input::releaseKey(e.key_code);
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


        RenderEngine::Renderer::setClearColor(m_colors[0], m_colors[1], m_colors[2], m_colors[3]);

        RenderEngine::Renderer::clearColor();

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(m_pWindow->get_size().x);
        io.DisplaySize.y = static_cast<float>(m_pWindow->get_size().y);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Background Color Window");
        ImGui::ColorEdit4("Background Color", m_colors);
        ImGui::SliderFloat3("Sprite position", m_sprite_pos, -50.f, 50.f);
        ImGui::SliderFloat3("Camera position", m_cam_pos, -50.f, 50.f);
        ImGui::SliderFloat3("Camera rotation", m_cam_rot, 0.f, 360.f);
        ImGui::Checkbox("Perspective camera", &m_isPerspectiveCam);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_cam->set_projection_mode(m_isPerspectiveCam ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);

        ResourceManager::getShaderProgram("spriteShader")->setMatrix4("view_projectionMat", m_cam->get_projection_matrix() * m_cam->get_view_matrix());


        m_pWindow->on_update();
        on_update(duration);
    }

    m_pWindow = nullptr;

    return 0;
}

void Application::on_update(const double delta)
{
    bool isMoveCam = false;
    glm::vec3 movement_delta{ 0,0,0 };
    glm::vec3 rotation_delta{ 0,0,0 };

    if (Input::isKeyPressed(KeyCode::KEY_W))
    {
        movement_delta.x += static_cast<float>(m_cam_velocity * delta);
        isMoveCam = true;
    }
    else if (Input::isKeyPressed(KeyCode::KEY_S))
    {
        movement_delta.x -= static_cast<float>(m_cam_velocity * delta);
        isMoveCam = true;
    }
    else if (Input::isKeyPressed(KeyCode::KEY_A))
    {
        movement_delta.y -= static_cast<float>(m_cam_velocity * delta);
        isMoveCam = true;
    }
    else if (Input::isKeyPressed(KeyCode::KEY_D))
    {
        movement_delta.y += static_cast<float>(m_cam_velocity * delta);
        isMoveCam = true;
    }
    else if (Input::isKeyPressed(KeyCode::KEY_SPACE))
    {
        movement_delta.z += static_cast<float>(m_cam_velocity * delta);
        isMoveCam = true;
    }
    else if (Input::isKeyPressed(KeyCode::KEY_LEFT_SHIFT))
    {
        movement_delta.z -= static_cast<float>(m_cam_velocity * delta);
        isMoveCam = true;
    }

    else if (Input::isKeyPressed(KeyCode::KEY_UP))
    {
        rotation_delta.y += static_cast<float>(m_cam_velocity * delta);
        isMoveCam = true;
    }
    else if (Input::isKeyPressed(KeyCode::KEY_DOWN))
    {
        rotation_delta.y -= static_cast<float>(m_cam_velocity * delta);
        isMoveCam = true;
    }
    else if (Input::isKeyPressed(KeyCode::KEY_LEFT))
    {
        rotation_delta.z -= static_cast<float>(m_cam_velocity * delta);
        isMoveCam = true;
    }
    else if (Input::isKeyPressed(KeyCode::KEY_RIGHT))
    {
        rotation_delta.z += static_cast<float>(m_cam_velocity * delta);
        isMoveCam = true;
    }
    else if (Input::isKeyPressed(KeyCode::KEY_Q))
    {
        rotation_delta.x += static_cast<float>(m_cam_velocity * delta);
        isMoveCam = true;
    }
    else if (Input::isKeyPressed(KeyCode::KEY_E))
    {
        rotation_delta.x -= static_cast<float>(m_cam_velocity * delta);
        isMoveCam = true;
    }
    if (isMoveCam) m_cam->add_movement_and_rotate(movement_delta, rotation_delta);
}
