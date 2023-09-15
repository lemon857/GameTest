#include "Games/GameApplication.h"

#include "EngineCore/System/Log.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Components/Highlight.h"
#include "EngineCore/Components/SpriteRenderer.h"
#include "EngineCore/Components/MeshRenderer.h"
#include "EngineCore/Light/DirectionalLight.h"
#include "EngineCore/Light/PointerLight.h"
#include "EngineCore/System/ShadersSettings.h"

#include "EngineCore/Window.h"

#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Input.h"

#include "EngineCore/Meshes/Cube.h"
#include "EngineCore/Meshes/Sprite.h"
#include "EngineCore/Meshes/ObjModel.h"
#include "EngineCore/Meshes/EmptyObject.h"
#include "EngineCore/Meshes/Plane.h"

int count = 0;

GameApp::GameApp()
	: Application()
{
}

GameApp::~GameApp()
{
	delete m_cam;
}
// инициализация, создание объектов
bool GameApp::init()
{
	m_cam = new Camera(glm::vec3(0), glm::vec3(0));

	m_cam->set_viewport_size(static_cast<float>(m_pWindow->get_size().x), static_cast<float>(m_pWindow->get_size().y));

    m_grid_line = new RenderEngine::Line(ResourceManager::getMaterial("default"));

	std::vector<std::string> names;

	names.push_back("default3DShader");

    //m_scene.add_object<ObjModel>("res/models/monkey.obj", ResourceManager::getMaterial("cube"));
	m_scene.add_object<Plane>(ResourceManager::getMaterial("cube"), glm::vec3(0.f), glm::vec3(10.f, 1.f, 10.f));
    m_scene.add_object<DirectionalLight>(names);
    m_scene.add_object<Cube>(ResourceManager::getMaterial("cube"));

    ((float*)ResourceManager::getMaterial("cube")->get_data("ambient_factor"))[0] = 0.4f;

    int colls = 10, rows = 10;

    const auto& transform = m_scene.at(0)->getComponent<Transform>();

    glm::vec3 size = glm::vec3(transform->get_scale().x / colls * 2, transform->get_scale().y, transform->get_scale().z / rows * 2);

    glm::vec3 startPos = transform->get_position() - transform->get_scale() + glm::vec3(1.f);

    glm::vec3 startPosLinesGrid = transform->get_position() - transform->get_scale() + glm::vec3(0.f, 1.5f, 0.f);

    for (int i = 0; i < colls; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            parts.push_back(startPos + glm::vec3((float)i * size.x, 0, (float)j * size.z));
            if (i == 0)
            {
                m_lines_grid_start_colls.push_back(startPosLinesGrid + glm::vec3((float)i * size.x, 0, (float)j * size.z));
            }
            if (j == 0)
            {
                m_lines_grid_start_rows.push_back(startPosLinesGrid + glm::vec3((float)i * size.x, 0, (float)j * size.z));
            }
            if (i == colls - 1)
            {
                m_lines_grid_end_colls.push_back(startPosLinesGrid + glm::vec3((float)(i + 1) * size.x, 0, (float)j * size.z));
            }
            if (j == rows - 1)
            {
                m_lines_grid_end_rows.push_back(startPosLinesGrid + glm::vec3((float)i * size.x, 0, (float)(j + 1) * size.z));
            }
        }
    }

    m_scene.at(2)->addComponent<Transform>();
    m_scene.at(2)->addComponent<Highlight>(ResourceManager::getMaterial("default"), true);

	return true;
}
// цикл для проерки нажатия клавиш
void GameApp::on_key_update(const double delta)
{
    glm::vec3 movement_delta{ 0,0,0 };
    glm::vec3 rotation_delta{ 0,0,0 };

    double addSpeed = 1;

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
// основной цикл движка
void GameApp::on_update(const double delta)
{
    RenderEngine::Renderer::setClearColor(m_colors[0], m_colors[1], m_colors[2], m_colors[3]);

    RenderEngine::Renderer::clearColor();

    ResourceManager::getShaderProgram("colorShader")->use();
    ResourceManager::getShaderProgram("colorShader")->setMatrix4(VIEW_PROJECTION_MATRIX_NAME, m_cam->get_projection_matrix() * m_cam->get_view_matrix());

    ResourceManager::getShaderProgram("default3DShader")->use();
    ResourceManager::getShaderProgram("default3DShader")->setVec3("cam_position", m_cam->get_position());

    m_scene.at(2)->getComponent<Transform>()->set_position(parts[count]);

    for (size_t i = 0; i < m_scene.get_list().size(); i++)
    {
        MeshRenderer* mesh = m_scene.get_list()[i]->object->getComponent<MeshRenderer>();
        if (mesh != nullptr)
        {
            std::shared_ptr<RenderEngine::ShaderProgram> shader = mesh->get_material_ptr()->get_shader_ptr();
            shader->use();
            shader->setMatrix4(VIEW_PROJECTION_MATRIX_NAME, m_cam->get_projection_matrix() * m_cam->get_view_matrix());
        }
        m_scene.get_list()[i]->object->update(delta);
    }

    for (int i = 0; i < m_lines_grid_start_colls.size(); i++)
    {
        m_grid_line->render_from_to(m_lines_grid_start_colls[i], m_lines_grid_end_colls[i], glm::vec3(1.f));
    }
    for (int i = 0; i < m_lines_grid_start_rows.size(); i++)
    {
        m_grid_line->render_from_to(m_lines_grid_start_rows[i], m_lines_grid_end_rows[i], glm::vec3(1.f));
    }
}
// инициализация эвентов
bool GameApp::init_events()
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
    m_event_dispather.add_event_listener<EventKeyReleased>([&](EventKeyReleased& e)
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

            LOG_INFO("[EVENT] Mouse moved to {0}x{1}", e.x, e.y);
        });
    m_event_dispather.add_event_listener<EventMouseScrolled>(
        [&](EventMouseScrolled& e)
        {
            if (count + e.y_offset >= 0) count += e.y_offset;
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
            m_init_mouse_pos_x = e.x_pos;
            m_init_mouse_pos_y = e.y_pos;
        });
    m_event_dispather.add_event_listener<EventMouseButtonReleased>([&](EventMouseButtonReleased& e)
        {
            LOG_INFO("[EVENT] Mouse button released at ({0}x{1})", e.x_pos, e.y_pos);
            Input::releaseMouseButton(e.mouse_button);
            m_init_mouse_pos_x = e.x_pos;
            m_init_mouse_pos_y = e.y_pos;
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
