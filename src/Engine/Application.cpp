#include "EngineCore/Application.h"
#include "EngineCore/Window.h"
#include "EngineCore/Input.h"
#include "EngineCore/System/Log.h"
#include "EngineCore/System/Stopwatch.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Components/SpriteRenderer.h"
#include "EngineCore/Renderer3D/GraphicsObject.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Components/Highlight.h"
#include "EngineCore/Light/DirectionalLight.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <chrono>

#include <glm/gtc/matrix_transform.hpp>

Stopwatch watch;

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
    watch.start();

    INIdata data{ window_size, m_window_position, m_maximized_window };
    ResourceManager::load_INI_settings("EngineTest.ini", data, false);

    m_pCloseWindow = false;
    m_pWindow = std::make_unique<Window>(title, m_window_position, window_size, m_maximized_window);

    ResourceManager::load_JSON_resources("res/resources.json");

    if (!init_events())
    {        
        return -1;
    }

    if (!init())
    {
        return -1;
    }

    LOG_INFO("Time for init: {0}", watch.stop());

    auto lastTime = std::chrono::high_resolution_clock::now();

    double curTime = 0;

    int curFpsEs = 0;
    int fps = 0;
    int countFpsEs = 3;

    std::vector<int> fpses{ 0, 0, 0 };

    while (!m_pCloseWindow)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
        lastTime = currentTime;

        RenderEngine::Renderer::setClearColor(m_colors[0], m_colors[1], m_colors[2], m_colors[3]);

        RenderEngine::Renderer::clearColor();

        glm::vec3 pos(0), rot(0);

        pos = m_cam->get_position();
        rot = m_cam->get_rotation();

        m_cam_pos[0] = pos.x;
        m_cam_pos[1] = pos.y;
        m_cam_pos[2] = pos.z;

        m_cam_rot[0] = rot.x;
        m_cam_rot[1] = rot.y;
        m_cam_rot[2] = rot.z;

        ResourceManager::getShaderProgram("colorShader")->use();
        ResourceManager::getShaderProgram("colorShader")->setMatrix4("view_projectionMat", m_cam->get_projection_matrix() * m_cam->get_view_matrix());
        ResourceManager::getShaderProgram("shape3DShader")->use();
        ResourceManager::getShaderProgram("shape3DShader")->setMatrix4("view_projectionMat", m_cam->get_projection_matrix() * m_cam->get_view_matrix());
        ResourceManager::getShaderProgram("spriteShader")->use();
        ResourceManager::getShaderProgram("spriteShader")->setMatrix4("view_projectionMat", m_cam->get_projection_matrix() * m_cam->get_view_matrix());

        if (m_drawNullIntersection)
        {
            m_line->render_from_to(glm::vec3(0.f, 0.f, 50.f), glm::vec3(0.f, 0.f, -50.f), glm::vec3(0.f, 0.f, 1.f));
            m_line->render_from_to(glm::vec3(0.f, 50.f, 0.f), glm::vec3(0.f, -50.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
            m_line->render_from_to(glm::vec3(50.f, 0.f, 0.f), glm::vec3(-50.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
        }

        if (m_objs[item_current]->getComponent<Transform>() != nullptr)
        {

            if (m_moveObject)
            {
                glm::mat4 translateMat(
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    pos[0], pos[1], pos[2], 1);

                glm::vec3 posfin = glm::vec3(translateMat * glm::vec4(glm::vec3(m_world_mouse_dir_x, m_world_mouse_dir_y, m_world_mouse_dir_z) * m_distance, 1.f));
                m_objs[item_current]->getComponent<Transform>()->set_position(posfin);
            }
            m_line->render(m_objs[item_current]->getComponent<Transform>()->get_position(), glm::vec3(5.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
            m_line->render(m_objs[item_current]->getComponent<Transform>()->get_position(), glm::vec3(0.f, 0.f, 5.f), glm::vec3(0.f, 0.f, 1.f));
            m_line->render(m_objs[item_current]->getComponent<Transform>()->get_position(), glm::vec3(0.f, 5.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        }

        for (size_t i = 0; i < m_objs.size(); i++)
        {
            m_objs[i]->update(duration);
        }

        // ------------------------------------------------------------ // 
        on_ui_render();
        // ------------------------------------------------------------ // 

        fps++;
        curTime += duration;

        if (curTime >= 1000)
        {
            fpses[curFpsEs] = fps;
            curFpsEs++;
            if (curFpsEs >= countFpsEs)
            {
                int sum = 0;
                for (int cur : fpses)
                {
                    sum += cur;
                }
                m_fps = (int)((float)sum / (float)countFpsEs);
                curFpsEs = 0;
            }
            curTime = 0;
            fps = 0;
        }

        m_pWindow->on_update();
        on_key_update(duration);
    }

    INIdata endData{ m_pWindow->get_size(), m_window_position, m_maximized_window};
    ResourceManager::load_INI_settings("EngineTest.ini", endData, true);
    m_pWindow = nullptr;

    return 0;
}

bool Application::init()
{
    m_cam = new Camera(glm::vec3(0), glm::vec3(0));

    m_cam->set_viewport_size(static_cast<float>(m_pWindow->get_size().x), static_cast<float>(m_pWindow->get_size().y));

    std::vector<std::string> names;

    names.push_back("shape3DShader");

    add_object<ObjModel>("res/models/monkey.obj", ResourceManager::getMaterial("monkey"));
    add_object<DirectionalLight>(names);
    //add_object<Cube>(ResourceManager::getMaterial("cube"));
    //add_object<Cube>(ResourceManager::getMaterial("default")); 
    //add_object<Sprite>(ResourceManager::getMaterial("cube"), "YellowUp11");

    m_line = new RenderEngine::Line(ResourceManager::getMaterial("default"));
    m_line_transform = new RenderEngine::Line(ResourceManager::getMaterial("default"), 10);

    return true;
}

bool Application::init_events()
{
    m_event_dispather.add_event_listener<EventWindowResize>([&](EventWindowResize& e)
        {
            LOG_INFO("[EVENT] Resize: {0}x{1}", e.width, e.height);
            if (e.width != 0 && e.height != 0)
            {
                RenderEngine::Renderer::setViewport(e.width, e.height);
                m_cam->set_viewport_size(e.width, e.height);
            }
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
    m_event_dispather.add_event_listener<EventMouseMoved>([&](EventMouseMoved& e)
        {
            glm::vec3 objcoord = m_cam->get_world_mouse_position(glm::vec2(e.x, e.y), m_pWindow->get_size());
                 
            glm::vec3 pos;

            pos.x = objcoord.x - m_cam->get_position().x;
            pos.y = objcoord.y - m_cam->get_position().y;
            pos.z = objcoord.z - m_cam->get_position().z;

            pos = glm::normalize(pos);

            m_world_mouse_dir_x = pos.x;
            m_world_mouse_dir_y = pos.y;
            m_world_mouse_dir_z = pos.z;

            m_mouse_pos_x = e.x;
            m_mouse_pos_y = e.y;
            LOG_INFO("[EVENT] Mouse moved to {0}x{1}", e.x, e.y);
        });
    m_event_dispather.add_event_listener<EventMouseScrolled>(
        [&](EventMouseScrolled& e)
        {
            if (m_distance + e.y_offset > 0) m_distance += e.y_offset;
            LOG_INFO("[EVENT] Scroll: {0}x{1}", e.x_offset, e.y_offset);
        });
    m_event_dispather.add_event_listener<EventWindowClose>([&](EventWindowClose& e)
        {
            LOG_INFO("[EVENT] Window close");
            m_pCloseWindow = true;
        });
    m_event_dispather.add_event_listener<EventMouseButtonPressed>([&](EventMouseButtonPressed& e)
        {
            LOG_INFO("[EVENT] Mouse button pressed at ({0}x{1})", e.x_pos, e.y_pos);
            Input::pressMouseButton(e.mouse_button);
            on_mouse_button_event(e.mouse_button, e.x_pos, e.y_pos, true);
        });
    m_event_dispather.add_event_listener<EventMouseButtonReleased>([&](EventMouseButtonReleased& e)
        {
            LOG_INFO("[EVENT] Mouse button released at ({0}x{1})", e.x_pos, e.y_pos);
            Input::releaseMouseButton(e.mouse_button);
            on_mouse_button_event(e.mouse_button, e.x_pos, e.y_pos, false);
        });
    m_event_dispather.add_event_listener<EventMaximizeWindow>([&](EventMaximizeWindow& e)
        {
            LOG_INFO("[EVENT] Maximized window: {0}", e.isMaximized);
            m_maximized_window = e.isMaximized;
        });
    m_event_dispather.add_event_listener<EventMoveWindow>([&](EventMoveWindow& e)
        {
            LOG_INFO("[EVENT] Move window to: {0}x{1}", e.x_pos, e.y_pos);
            m_window_position = glm::ivec2(e.x_pos, e.y_pos);
        });
    m_pWindow->set_event_callback(
        [&](BaseEvent& e)
        {
            m_event_dispather.dispatch(e);
        });
    return true;
}

void Application::on_key_update(const double delta)
{
    if (m_isUIhovered) return;
    glm::vec3 movement_delta{ 0,0,0 };
    glm::vec3 rotation_delta{ 0,0,0 };

    double addSpeed = 1;

    if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
    {
        //m_moveObject = !m_moveObject;
        int info = RenderEngine::Renderer::get_info_pixel(m_mouse_pos_x, m_mouse_pos_y, m_pWindow->get_size().y);
        LOG_INFO("ObjectID: {0}", info);
    }

    if (Input::isKeyPressed(KeyCode::KEY_LEFT_CONTROL))
    {
        addSpeed = m_add_ctrl_speed;
    }

    if (Input::isKeyPressed(KeyCode::KEY_W))
    {
        movement_delta.z += static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_S))
    {
        movement_delta.z -= static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_A))
    {
        movement_delta.x -= static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_D))
    {
        movement_delta.x += static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_SPACE))
    {
        movement_delta.y += static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_LEFT_SHIFT))
    {
        movement_delta.y -= static_cast<float>(addSpeed * m_cam_velocity * delta);
    }

    else if (Input::isKeyPressed(KeyCode::KEY_UP))
    {
        rotation_delta.x += static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_DOWN))
    {
        rotation_delta.x -= static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_LEFT))
    {
        rotation_delta.y -= static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_RIGHT))
    {
        rotation_delta.y += static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_Q))
    {
        rotation_delta.z += static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_E))
    {
        rotation_delta.z -= static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }

    if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_MIDDLE))
    {
       glm::vec2 pos = m_pWindow->get_current_cursor_position();
       rotation_delta.y = m_isInversiveMouseY 
           ? (rotation_delta.y + ((m_init_mouse_pos_x - pos.x) / (1 / m_cam_sensetivity)))
           : (rotation_delta.y - ((m_init_mouse_pos_x - pos.x) / (1 / m_cam_sensetivity)));
       rotation_delta.x += (m_init_mouse_pos_y - pos.y) / (1 / m_cam_sensetivity);
       m_init_mouse_pos_x = pos.x;
       m_init_mouse_pos_y = pos.y;
    }

    m_cam->add_movement_and_rotation(movement_delta, rotation_delta);
}

void Application::on_mouse_button_event(const MouseButton button, const double pos_x, const double pos_y, const bool isPressed)
{
    m_init_mouse_pos_x = pos_x;
    m_init_mouse_pos_y = pos_y;
}