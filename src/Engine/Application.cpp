#include "EngineCore/Application.h"
#include "EngineCore/Window.h"
#include "EngineCore/Input.h"

#include "EngineCore/System/Log.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Components/SpriteRenderer.h"
#include "EngineCore/Renderer3D/GraphicsObject.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Components/Highlight.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <chrono>

#include <glm/gtc/matrix_transform.hpp>

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

        m_line->render(glm::vec3(0.f), glm::vec3(m_line_pos[0], m_line_pos[1], m_line_pos[2]), glm::vec3(1.f));

        //m_line->render_from_to(m_cam->get_position() - glm::vec3(1.f), glm::vec3(m_world_mouse_pos_x, m_world_mouse_pos_y, m_world_mouse_pos_z), glm::vec3(1.f));

        //m_line->render_from_to(glm::vec3(0.f), glm::vec3(m_world_mouse_pos_x, m_world_mouse_pos_y, m_world_mouse_pos_z), glm::vec3(1.f));

        if (m_drawNullIntersection)
        {
            m_line->render_from_to(glm::vec3(0.f, 0.f, 50.f), glm::vec3(0.f, 0.f, -50.f), glm::vec3(0.f, 0.f, 1.f));
            m_line->render_from_to(glm::vec3(0.f, 50.f, 0.f), glm::vec3(0.f, -50.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
            m_line->render_from_to(glm::vec3(50.f, 0.f, 0.f), glm::vec3(-50.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
        }

        ResourceManager::getShaderProgram("shape3DShader")->use();
        ResourceManager::getShaderProgram("shape3DShader")->setVec3("light_color", glm::vec3(m_light_color[0], m_light_color[1], m_light_color[2]));
        ResourceManager::getShaderProgram("shape3DShader")->setVec3("light_position", glm::vec3(m_light_pos[0], m_light_pos[1], m_light_pos[2]));
        ResourceManager::getShaderProgram("shape3DShader")->setVec3("cam_position", m_cam->get_position());
        ResourceManager::getShaderProgram("shape3DShader")->setFloat("ambient_factor", m_ambient_factor);
        ResourceManager::getShaderProgram("shape3DShader")->setFloat("diffuse_factor", m_diffuse_factor);
        ResourceManager::getShaderProgram("shape3DShader")->setFloat("specular_factor", m_specular_factor);
        ResourceManager::getShaderProgram("shape3DShader")->setFloat("shininess", m_shininess);
        ResourceManager::getShaderProgram("shape3DShader")->setFloat("metalic_factor", m_metalic_factor);

        ResourceManager::getShaderProgram("colorShader")->use();
        ResourceManager::getShaderProgram("colorShader")->setVec3("sourceColor", glm::vec3(m_light_color[0], m_light_color[1], m_light_color[2]));

        m_cube->update(duration);
        m_light_source->update(duration);
        m_sprite->update(duration);
        m_model->update(duration);

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

    auto pShapeProgram = ResourceManager::getShaderProgram("colorShader");
    auto pSpriteProgram = ResourceManager::getShaderProgram("spriteShader");

    m_model = new ObjModel("res/models/monkey.obj", ResourceManager::getTexture("CottageTexture"), ResourceManager::getShaderProgram("shape3DShader"));

    m_line = new RenderEngine::Line(pShapeProgram);

    m_cube = new Cube(ResourceManager::getTexture("CubeTexture"), "default", ResourceManager::getShaderProgram("shape3DShader"));
    m_light_source = new Cube(nullptr, "default", pShapeProgram);
    m_sprite = new Sprite(ResourceManager::getTexture("TanksTextureAtlas"), "YellowUp11", pSpriteProgram);

    m_cube->addComponent<Highlight>(pShapeProgram, glm::vec3(1.f));
    m_sprite->addComponent<Highlight>(pShapeProgram, glm::vec3(1.f));

    m_cube->addComponent<Transform>(glm::vec3(m_cube_pos[0], m_cube_pos[1], m_cube_pos[2]),
        glm::vec3(m_cube_scale[0], m_cube_scale[1], m_cube_scale[2]),
        glm::vec3(m_cube_rot[0], m_cube_rot[1], m_cube_rot[2]));
    m_sprite->addComponent<Transform>(glm::vec3(m_SpriteRenderer_pos[0], m_SpriteRenderer_pos[1], m_SpriteRenderer_pos[2]),
        glm::vec3(m_SpriteRenderer_scale[0], m_SpriteRenderer_scale[1], m_SpriteRenderer_scale[2]),
        glm::vec3(m_SpriteRenderer_rot[0], m_SpriteRenderer_rot[1], m_SpriteRenderer_rot[2]));
    m_light_source->addComponent<Transform>(glm::vec3(m_light_pos[0], m_light_pos[1], m_light_pos[2]), glm::vec3(0.1f), glm::vec3(0.f));

    m_model->addComponent<Transform>(glm::vec3(5.f, 0.f, 0.f), glm::vec3(0.5f), glm::vec3(1.f));
    m_model->addComponent<Highlight>(pShapeProgram, glm::vec3(1.f));

    m_cube->setSubTexture("BrickWall");


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

            m_world_mouse_pos_x = objcoord.x;
            m_world_mouse_pos_y = objcoord.y;
            m_world_mouse_pos_z = objcoord.z;

            m_mouse_pos_x = e.x;
            m_mouse_pos_y = e.y;
            LOG_INFO("[EVENT] Mouse moved to {0}x{1}", e.x, e.y);
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
    glm::vec3 movement_delta{ 0,0,0 };
    glm::vec3 rotation_delta{ 0,0,0 };

    double addSpeed = 1;

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

    if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_RIGHT))
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