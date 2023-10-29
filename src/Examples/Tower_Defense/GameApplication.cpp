#include "Games/Tower_Defense/GameApplication.h"

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
#include "EngineCore/Renderer3D/GraphicsObject.h"
#include "EngineCore/Input.h"

#include "EngineCore/Meshes/EmptyObject.h"
#include "EngineCore/Meshes/Cube.h"
#include "EngineCore/Meshes/ObjModel.h"
#include "EngineCore/Meshes/EmptyObject.h"
#include "EngineCore/Meshes/Plane.h"
#include "EngineCore/Meshes/Grid.h" 

#include "EngineCore/GUI/GUI_place.h"
#include "EngineCore/GUI/Square.h"
#include "EngineCore/GUI/FontRenderer.h"

#include <array>
#include <memory>

#define MIN_DISTANCE_TO_ENEMY 7.f

const int size_x = 30, size_y = 30;

std::array <bool, size_x * size_y> map;

int cur = 0;
int curObj = 3;

GUI::FontRenderer* text;
int countEnemies = 0;

bool is_event_logging_active = false;

bool is_grid_active = false;

bool isKeyPressed = false;

unsigned int countKills = 0;
unsigned int fps = 0;
unsigned int frames = 0;
double times = 0;

GUI::GUI_place* m_gui_place;

GameApp::GameApp()
	: Application()
{
}

GameApp::~GameApp()
{
    delete m_cam;
}

bool GameApp::init()
{
    text = new GUI::FontRenderer(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"));

    m_cam = new Camera(glm::vec3(0), glm::vec3(0));

    m_cam->set_viewport_size(static_cast<float>(m_pWindow->get_size().x), static_cast<float>(m_pWindow->get_size().y));

    m_gui_place = new GUI::GUI_place(m_cam, ResourceManager::getMaterial("default"));

    auto square = new GUI::Square(ResourceManager::getMaterial("default"), glm::vec2(100.f), glm::vec2(100.f));

    square->set_click_callback([]() 
        {
            LOG_INFO("EEE HELLO WORLD YEPTA");
        });

    m_gui_place->add_element(square);

    //m_line = new RenderEngine::Line(ResourceManager::getMaterial("default"));
  
    map.fill(false);

	std::vector<std::string> names;

  names.push_back("default3DShader");

	m_scene.add_object<Plane>(ResourceManager::getMaterial("dirt"), glm::vec3(size_x, 0.f, size_y), glm::vec3(size_x, 0.f, size_y));
    m_scene.add_object<DirectionalLight>(names);
    m_scene.add_object<Grid>(glm::vec3(size_x, 0.5f, size_y), glm::vec2(1.f), size_x, size_y, glm::vec3(1.f), ResourceManager::getMaterial("default"));
    m_scene.add_object<EmptyObject>();
    //m_scene.add_object<Cube>(ResourceManager::getMaterial("cube"));

    ((float*)ResourceManager::getMaterial("cube")->get_data("ambient_factor"))[0] = 0.3f;

    ((float*)ResourceManager::getMaterial("dirt")->get_data("ambient_factor"))[0] = 0.25f;
    ((float*)ResourceManager::getMaterial("dirt")->get_data("diffuse_factor"))[0] = 0.1f;
    ((float*)ResourceManager::getMaterial("dirt")->get_data("specular_factor"))[0] = 0.0f;
    ((float*)ResourceManager::getMaterial("dirt")->get_data("metalic_factor"))[0] = 0.0f;
    ((float*)ResourceManager::getMaterial("dirt")->get_data("shininess"))[0] = 0.1f;

    ((float*)ResourceManager::getMaterial("tower")->get_data("ambient_factor"))[0] = 0.3f;
    ((float*)ResourceManager::getMaterial("tower")->get_data("diffuse_factor"))[0] = 0.4f;
    ((float*)ResourceManager::getMaterial("tower")->get_data("specular_factor"))[0] = 0.0f;
    ((float*)ResourceManager::getMaterial("tower")->get_data("metalic_factor"))[0] = 0.0f;
    ((float*)ResourceManager::getMaterial("tower")->get_data("shininess"))[0] = 0.1f;

    ((float*)ResourceManager::getMaterial("castle")->get_data("ambient_factor"))[0] = 0.3f;
    ((float*)ResourceManager::getMaterial("castle")->get_data("diffuse_factor"))[0] = 0.4f;
    ((float*)ResourceManager::getMaterial("castle")->get_data("specular_factor"))[0] = 0.0f;
    ((float*)ResourceManager::getMaterial("castle")->get_data("metalic_factor"))[0] = 0.0f;
    ((float*)ResourceManager::getMaterial("castle")->get_data("shininess"))[0] = 0.1f;

    ((float*)ResourceManager::getMaterial("monkey")->get_data("ambient_factor"))[0] = 0.5f;

    m_scene.at(curObj)->addComponent<Transform>();
    m_scene.at(curObj)->addComponent<Highlight>(ResourceManager::getMaterial("default"), true); 

    const auto& transform = m_scene.at(0)->getComponent<Transform>();

    glm::vec3 size = glm::vec3(transform->get_scale().x / size_x * 2, transform->get_scale().y, transform->get_scale().z / size_y * 2);

    glm::vec3 startPos = transform->get_position() - transform->get_scale() + glm::vec3(1.f);

    for (int i = 0; i < size_x; i++)
    {
        for (int j = 0; j < size_y; j++)
        {
            parts.push_back(startPos + glm::vec3((float)i * size.x, 0, (float)j * size.z));
        }
    }

    ResourceManager::load_OBJ_file("res/models/castle.obj");
    ResourceManager::load_OBJ_file("res/models/tower.obj");
    ResourceManager::load_OBJ_file("res/models/monkey.obj");

    m_main_castle = new Castle(parts[int((size_x * size_y) / 2) + int(size_x / 2)], 100,
        "res/models/castle.obj", ResourceManager::getMaterial("castle"), ResourceManager::getMaterial("default"));
    map[int((size_x * size_y) / 2) + int(size_x / 2)] = true;

    return true;
}

void GameApp::on_key_update(const double delta)
{
    glm::vec3 movement_delta{ 0,0,0 };
    glm::vec3 rotation_delta{ 0,0,0 };

    double addSpeed = 1;
    if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
    {
        int x = (int)floor(m_world_mouse_pos_x / 2.f);
        int y = (int)floor(m_world_mouse_pos_z / 2.f);

        if (x * y <= (size_x - 1) * (size_y - 1))
        {
            cur = x * size_y + y;
        }
    }
    else if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_RIGHT))
    {
        if (!map[cur])
        {
            map[cur] = true;

            m_towers.push_back(new BaseTower("res/models/tower.obj",
                ResourceManager::getMaterial("tower"), nullptr, parts[cur], 1, new RenderEngine::Line(ResourceManager::getMaterial("default"))));

            //m_scene.at(curObj)->deleteComponent<Highlight>();
            //curObj++;
            // 
            //m_scene.add_object<ObjModel>("res/models/monkey.obj", ResourceManajger::getMaterial("monkey"));
            //m_scene.add_object<Cube>(ResourceManager::getMaterial("cube"));

            //m_scene.at(curObj)->addComponent<Transform>();
            //m_scene.at(curObj)->addComponent<Highlight>(ResourceManager::getMaterial("default"), true);
            LOG_INFO("Add tower at {0}x{1}", parts[cur].x, parts[cur].z);
        }
    }
    if (Input::isKeyPressed(KeyCode::KEY_G))
    {
        if (is_grid_active && !isKeyPressed)
        {
            is_grid_active = false;
            isKeyPressed = true;
        }
        else if (!isKeyPressed)
        {
            is_grid_active = true;
            isKeyPressed = true;
        }
    }
    else if (Input::isKeyPressed(KeyCode::KEY_L))
    {
        if (is_event_logging_active && !isKeyPressed)
        {
            is_event_logging_active = false;
            isKeyPressed = true;
        }
        else if (!isKeyPressed)
        {
            is_event_logging_active = true;
            isKeyPressed = true;
        }
    }
    else if (Input::isKeyPressed(KeyCode::KEY_J))
    {
        if (!isKeyPressed)
        {
            countEnemies++;
            isKeyPressed = true;
        }
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

void GameApp::on_update(const double delta)
{
    // clear screen
    RenderEngine::Renderer::setClearColor(m_colors[0], m_colors[1], m_colors[2], m_colors[3]);

    RenderEngine::Renderer::clearColor();
    // set matrix
    ResourceManager::getShaderProgram("colorShader")->use();
    ResourceManager::getShaderProgram("colorShader")->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_cam->get_projection_matrix() * m_cam->get_view_matrix());
    //ResourceManager::getShaderProgram("colorShader")->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_cam->get_ui_matrix());

    ResourceManager::getShaderProgram("default3DShader")->use();
    ResourceManager::getShaderProgram("default3DShader")->setVec3("cam_position", m_cam->get_position());

    m_gui_place->on_update(delta);

    m_scene.at(curObj)->getComponent<Transform>()->set_position(parts[cur]); 
    //m_grid_line->render(glm::vec3(0), glm::vec3(m_world_mouse_pos_x, m_world_mouse_pos_y + 0.1f, m_world_mouse_pos_z), glm::vec3(1.f));

    for (size_t i = 0; i < m_scene.get_list().size(); i++)
    {
        MeshRenderer* mesh = m_scene.at(i)->getComponent<MeshRenderer>();
        if (mesh != nullptr)
        {
            std::shared_ptr<RenderEngine::ShaderProgram> shader = mesh->get_material_ptr()->get_shader_ptr();
            shader->use();

            shader->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_cam->get_projection_matrix() * m_cam->get_view_matrix());
        }
        if (i != 2) m_scene.at(i)->update(delta);
        if (i == 2 && is_grid_active) m_scene.at(i)->update(delta);
    }       

    if (m_isLose) return;

    m_main_castle->update(delta);

    if (m_main_castle->isDestroyed() && !m_isLose)
    {
        LOG_CRIT("You lose!!! You score: {0}", countKills);
        m_isLose = true;
        m_pCloseWindow = true;
    }

    for (; countEnemies > 0; countEnemies--)
    {
        short snum = rand() % 4;
        int spawn = 0;        
        if (snum == 0) spawn = rand() % size_x;
        else if (snum == 1) spawn = (rand() % size_x) + ((size_x - 1) * size_y);
        else if (snum == 2) spawn = (rand() % size_y) * size_x;
        else if (snum == 3) spawn = ((rand() % size_y) * size_x) + (size_x - 1);
        m_enemies.push_back(new Enemy(new ObjModel("res/models/monkey.obj", ResourceManager::getMaterial("monkey")),
            m_main_castle, parts[spawn], 1, 0.005, 50, ResourceManager::getMaterial("default")));
    }

    for (auto curTower : m_towers)
    {
        unsigned int distance = 100;
        for (size_t i = 0; i < m_enemies.size(); i++)
        {
            if (m_enemies[i] == nullptr) continue;
            glm::vec3 a = m_enemies[i]->get_pos() - curTower->get_pos();
            if (sqrt(a.x * a.x + a.z * a.z) < MIN_DISTANCE_TO_ENEMY && sqrt(a.x * a.x + a.z * a.z) < distance)
            {
                distance = sqrt(a.x * a.x + a.z * a.z);
                curTower->set_target(m_enemies[i]);
            }
        }
        curTower->update(delta);
    }

    for (size_t i = 0; i < m_enemies.size(); i++)
    {
        if (m_enemies[i] == nullptr) continue;        
        if (m_enemies[i]->is_destroy())
        {
            countEnemies++;
            for (auto curTower : m_towers)
            {
                if (curTower->get_target() == m_enemies[i]) curTower->set_target(nullptr);
            }
            delete m_enemies[i];
            m_enemies.remove(i);
            countKills++;
            continue;
        }
        m_enemies[i]->update(delta);        
    }

    if (frames < 5)
    {
        frames++;
        times += delta;
    }
    else
    {
        fps = int((frames / times) * 1000.f);
        frames = 0;
        times = 0;
    }
}

void GameApp::on_ui_render()
{
    text->render_text("Kills: " + std::to_string(countKills), 10.f, 950.f, 1.f, glm::vec3(1.f, 0.f, 0.f), m_cam->get_ui_matrix());
    text->render_text("FPS: " + std::to_string(fps), 10.f, 900.f, 1.f, glm::vec3(1.f, 0.33f, 0.33f), m_cam->get_ui_matrix());
    m_gui_place->on_render();
}

bool GameApp::init_events()
{
    m_event_dispather.add_event_listener<EventWindowResize>([&](EventWindowResize& e)
        {
            if (is_event_logging_active) LOG_INFO("[EVENT] Resize: {0}x{1}", e.width, e.height);
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

                  if (is_event_logging_active) LOG_INFO("[EVENT] Key repeated {0}", static_cast<char>(e.key_code));
                }
                else
                {
                   if (is_event_logging_active) LOG_INFO("[EVENT] Key pressed {0}", static_cast<char>(e.key_code));
                }
            }
            Input::pressKey(e.key_code);
        });
    m_event_dispather.add_event_listener<EventKeyReleased>([&](EventKeyReleased& e)
        {
            if (e.key_code <= KeyCode::KEY_Z)  if (is_event_logging_active) LOG_INFO("[EVENT] Key released {0}", static_cast<char>(e.key_code));
            isKeyPressed = false;
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
            if (is_event_logging_active) LOG_INFO("[EVENT] Mouse moved to {0}x{1}", e.x, e.y);
        });
    m_event_dispather.add_event_listener<EventMouseScrolled>(
        [&](EventMouseScrolled& e)
        {
            m_gui_place->get_element(0)->add_position(glm::vec2(0.f, e.y_offset * 2.f));

            if (is_event_logging_active) LOG_INFO("[EVENT] Scroll: {0}x{1}", e.x_offset, e.y_offset);
        });
    m_event_dispather.add_event_listener<EventWindowClose>([&](EventWindowClose& e)
        {
            if (is_event_logging_active) LOG_INFO("[EVENT] Window close");
            m_pCloseWindow = true;
        });
    m_event_dispather.add_event_listener<EventMouseButtonPressed>([&](EventMouseButtonPressed& e)
        {

            if (is_event_logging_active) LOG_INFO("[EVENT] Mouse button pressed at ({0}x{1})", e.x_pos, e.y_pos);
            Input::pressMouseButton(e.mouse_button);
            m_init_mouse_pos_x = e.x_pos;
            m_init_mouse_pos_y = e.y_pos;
        });
    m_event_dispather.add_event_listener<EventMouseButtonReleased>([&](EventMouseButtonReleased& e)
        {
            if (is_event_logging_active) LOG_INFO("[EVENT] Mouse button released at ({0}x{1})", e.x_pos, e.y_pos);
            Input::releaseMouseButton(e.mouse_button);
            m_init_mouse_pos_x = e.x_pos;
            m_init_mouse_pos_y = e.y_pos;
        });
    m_event_dispather.add_event_listener<EventMaximizeWindow>([&](EventMaximizeWindow& e)
        {
            if (is_event_logging_active) LOG_INFO("[EVENT] Maximized window: {0}", e.isMaximized);
            m_maximized_window = e.isMaximized;
        });
    m_event_dispather.add_event_listener<EventMoveWindow>([&](EventMoveWindow& e)
        {
            if (is_event_logging_active) LOG_INFO("[EVENT] Move window to: {0}x{1}", e.x_pos, e.y_pos);
            m_window_position = glm::ivec2(e.x_pos, e.y_pos);
        });
    m_pWindow->set_event_callback(
        [&](BaseEvent& e)
        {
            m_event_dispather.dispatch(e);
        });
    return true;
}
