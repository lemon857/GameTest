
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

#include "EngineCore/Renderer3D/GraphicsObject.h"

#include "EngineCore/Meshes/EmptyObject.h"
#include "EngineCore/Meshes/Cube.h"
#include "EngineCore/Meshes/ObjModel.h"
#include "EngineCore/Meshes/EmptyObject.h"
#include "EngineCore/Meshes/Plane.h"
#include "EngineCore/Meshes/Grid.h" 

#include "EngineCore/GUI/GUI_place.h"
#include "EngineCore/GUI/Square.h"
#include "EngineCore/GUI/TextRenderer.h"
#include "EngineCore/GUI/Button.h"
#include "EngineCore/GUI/Sprite.h"
#include "EngineCore/GUI/InputField.h"
#include "EngineCore/GUI/CheckBox.h"

#include <array>
#include <memory>

#define MIN_DISTANCE_TO_BaseEnemy 7.f

#define CHECK_AVAILABLE_POS(x, y, width, height) ((x > 0 && x < width) && (y > 0 && y < height))

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
    m_cam = new Camera(glm::vec3(0), glm::vec3(0));

    m_cam->set_viewport_size(static_cast<float>(m_pWindow->get_size().x), static_cast<float>(m_pWindow->get_size().y));
       
    ((float*)ResourceManager::getMaterial("dirt")->get_data("ambient_factor"))[0] = 0.25f; 
    ((float*)ResourceManager::getMaterial("dirt")->get_data("diffuse_factor"))[0] = 0.1f;
    ((float*)ResourceManager::getMaterial("dirt")->get_data("specular_factor"))[0] = 0.0f;
    ((float*)ResourceManager::getMaterial("dirt")->get_data("metalic_factor"))[0] = 0.0f;

    ((float*)ResourceManager::getMaterial("tower")->get_data("ambient_factor"))[0] = 0.3f;
    ((float*)ResourceManager::getMaterial("tower")->get_data("diffuse_factor"))[0] = 0.4f;
    ((float*)ResourceManager::getMaterial("tower")->get_data("specular_factor"))[0] = 0.0f;
    ((float*)ResourceManager::getMaterial("tower")->get_data("metalic_factor"))[0] = 0.0f;

    ((float*)ResourceManager::getMaterial("castle")->get_data("ambient_factor"))[0] = 0.3f;
    ((float*)ResourceManager::getMaterial("castle")->get_data("diffuse_factor"))[0] = 0.4f;
    ((float*)ResourceManager::getMaterial("castle")->get_data("specular_factor"))[0] = 0.0f;
    ((float*)ResourceManager::getMaterial("castle")->get_data("metalic_factor"))[0] = 0.0f;

    ((float*)ResourceManager::getMaterial("monkey")->get_data("ambient_factor"))[0] = 0.5f;
    ((float*)ResourceManager::getMaterial("monkey")->get_data("diffuse_factor"))[0] = 0.4f;
    ((float*)ResourceManager::getMaterial("monkey")->get_data("specular_factor"))[0] = 0.0f;
    ((float*)ResourceManager::getMaterial("monkey")->get_data("metalic_factor"))[0] = 0.0f;
    ((float*)ResourceManager::getMaterial("monkey")->get_data("shininess"))[0] = 0.1f;

    std::vector<std::string> names;
    names.push_back("default3DShader");

    m_scene.add_object<Plane>(ResourceManager::getMaterial("dirt"), glm::vec3(size_x, 0.f, size_y), glm::vec3(size_x, 0.f, size_y));
    m_scene.add_object<DirectionalLight>(names);
    m_scene.add_object<Grid>(glm::vec3(size_x, 0.5f, size_y), glm::vec2(1.f), size_x, size_y, glm::vec3(1.f), ResourceManager::getMaterial("default"));
    m_scene.add_object<EmptyObject>();

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
    
    init_gui();
    start_game();

    return true;
}

void GameApp::on_key_update(const double delta)
{
    if (m_isLose || m_gui_place_menu->get_element("InputTest")->lead<GUI::InputField>()->get_focus()) return;

    glm::vec3 movement_delta{ 0,0,0 };
    glm::vec3 rotation_delta{ 0,0,0 };

    double addSpeed = 1;
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
            m_gui_place_menu->set_logging_active(false);
            isKeyPressed = true;
        }
        else if (!isKeyPressed)
        {
            is_event_logging_active = true;
            m_gui_place_menu->set_logging_active(true);
            isKeyPressed = true;
        }
    }
    else if (Input::isKeyPressed(KeyCode::KEY_J))
    {
        if (!isKeyPressed)
        {
            countEnemies++;
            countEnemiesPerm++;
            m_gui_debug->get_element("enemies")->lead<GUI::TextRenderer>()->set_text("Enemies: " + std::to_string(countEnemiesPerm));
            isKeyPressed = true;
        }
    }
    else if (Input::isKeyPressed(KeyCode::KEY_ESCAPE))
    {
        if (is_gui_active && !isKeyPressed)
        {
            if (gui_window != main)
            {
                gui_window = GUI_Active::main;
                m_gui_place_menu->set_active(true);
                m_gui_place_settings->set_active(false);
            }
            else
            {
                gui_window = GUI_Active::null;
                m_gui_place_menu->set_active(false);
                m_gui_place_settings->set_active(false);
                is_gui_active = false;
            }
            isKeyPressed = true;
        }
        else if (!isKeyPressed)
        {
            gui_window = GUI_Active::main;
            m_gui_place_menu->set_active(true);
            is_gui_active = true;
            m_gui_place_menu->set_active(true);
            isKeyPressed = true;
        }
    }
    else if (Input::isKeyPressed(KeyCode::KEY_F3))
    {
        if (is_debug_active && !isKeyPressed)
        {
            is_debug_active = false;
            isKeyPressed = true;
        }
        else if (!isKeyPressed)
        {
            is_debug_active = true;
            isKeyPressed = true;
        }
    }

    if (is_gui_active) return;

    if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT) && !m_gui_place_menu->get_focus())

    {
        int x = (int)floor(m_world_mouse_pos_x / 2.f);
        int y = (int)floor(m_world_mouse_pos_z / 2.f);

        if ((x * y <= (size_x - 1) * (size_y - 1)) && (x >= 0 && y >= 0))
        {
            if (x <= (size_x - 1) && y <= (size_y - 1)) cur = x * size_y + y;

        }
    }
    else if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_RIGHT))
    {
        if (!map[cur] && !isKeyPressedmouse)
        {
            int x = cur / size_y;
            int y = cur % size_x;

            bool spawn = false;

            /*if (CHECK_AVAILABLE_POS(x + 1, y, size_x, size_y))
            {
                spawn |= map[(x + 1) * size_y + y];
            }
            if (CHECK_AVAILABLE_POS(x - 1, y, size_x, size_y))
            {
                spawn |= map[(x - 1) * size_y + y];
            }
            if (CHECK_AVAILABLE_POS(x, y + 1, size_x, size_y))
            {
                spawn |= map[x * size_y + y + 1];
            }
            if (CHECK_AVAILABLE_POS(x, y - 1, size_x, size_y))
            {
                spawn |= map[x * size_y + y - 1];
            }*/
            if (!spawn)
            {
                map[cur] = true;
                isKeyPressedmouse = true;

                m_towers.push_back(new BaseTower("res/models/tower.obj",
                    ResourceManager::getMaterial("tower"), nullptr, parts[cur], 1, new RenderEngine::Line(ResourceManager::getMaterial("default"))));

                //m_scene.at(curObj)->deleteComponent<Highlight>();
                //curObj++;
                // 
                //m_scene.add_object<ObjModel>("res/models/monkey.obj", ResourceManajger::getMaterial("monkey"));
                //m_scene.add_object<Cube>(ResourceManager::getMaterial("cube"));

                //m_scene.at(curObj)->addComponent<Transform>();
                //m_scene.at(curObj)->addComponent<Highlight>(ResourceManager::getMaterial("default"), true);
                LOG_INFO("Add tower at {0}x{1}", x, y);
            }
        }
        /*else if (!isKeyPressedmouse)
        {
            isKeyPressedmouse = true;
            m_towers.push_back(new BaseTower("res/models/tower.obj",
                ResourceManager::getMaterial("tower"), nullptr, parts[cur] + glm::vec3(0.f, 2.f, 0.f), 1, new RenderEngine::Line(ResourceManager::getMaterial("default"))));
            LOG_INFO("Add tower on tower at {0}x{1}", parts[cur].x, parts[cur].z);
        }*/
    }

    if (Input::isKeyPressed(KeyCode::KEY_W))
    {
        movement_delta.z += static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    if (Input::isKeyPressed(KeyCode::KEY_S))
    {
        movement_delta.z -= static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    if (Input::isKeyPressed(KeyCode::KEY_A))
    {
        movement_delta.x -= static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    if (Input::isKeyPressed(KeyCode::KEY_D))
    {
        movement_delta.x += static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    if (Input::isKeyPressed(KeyCode::KEY_SPACE))
    {
        movement_delta.y += static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    if (Input::isKeyPressed(KeyCode::KEY_LEFT_SHIFT))

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

    m_gui_place_menu->on_update(delta);

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
        m_isLose = true;
        m_gui_place_menu->set_active(false);
        m_gui_place_settings->set_active(false);
        gui_window = null;
        m_gui->get_element("Lose scoreboard")->lead<GUI::TextRenderer>()->set_text("Your score: " + std::to_string(countKills));
        m_gui->set_active(true);
    }

    for (; countEnemies > 0; countEnemies--)
    {
        short snum = rand() % 4;
        int spawn = 0;        
        if (snum == 0) spawn = rand() % size_x;
        else if (snum == 1) spawn = (rand() % size_x) + ((size_x - 1) * size_y);
        else if (snum == 2) spawn = (rand() % size_y) * size_x;
        else if (snum == 3) spawn = ((rand() % size_y) * size_x) + (size_x - 1);
        auto a = new BaseEnemy(new ObjModel("res/models/monkey.obj", ResourceManager::getMaterial("monkey")),
            m_main_castle, parts[spawn], 1, 0.007, 50, ResourceManager::getMaterial("default"));
        m_enemies.push_back(a);
    }

    for (auto curTower : m_towers)
    {
        double distance = 100;
        for (size_t i = 0; i < m_enemies.size(); i++)
        {
            if (m_enemies[i] == nullptr) continue;
            glm::vec3 a = m_enemies[i]->get_pos() - curTower->get_pos();
            double d = sqrt(a.x * a.x + a.z * a.z);
            if (d < MIN_DISTANCE_TO_BaseEnemy && d < distance)
            {
                distance = d;
                if (curTower->get_target() != m_enemies[i]) curTower->set_target(m_enemies[i]);
            }
            else 
            {
                curTower->set_target(nullptr);
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
            m_gui_debug->get_element("kills")->lead<GUI::TextRenderer>()->set_text("Kills: " + std::to_string(countKills));
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
        m_gui_debug->get_element("fps")->lead<GUI::TextRenderer>()->set_text("FPS: " + std::to_string(fps));
        frames = 0;
        times = 0;
    }
}

void GameApp::on_ui_render()
{
    m_gui->on_render();
    if (is_debug_active) m_gui_debug->on_render();
    if (is_gui_active)
    {
        if (gui_window == main) m_gui_place_menu->on_render();
        else if (gui_window == settings) m_gui_place_settings->on_render();
    }
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
                m_gui->on_resize();
                m_gui_debug->on_resize();
                m_gui_place_menu->on_resize();
                m_gui_place_settings->on_resize();

            }
        });
    m_event_dispather.add_event_listener<EventKeyPressed>([&](EventKeyPressed& e)
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
            m_gui_place_menu->get_element("InputTest")->lead<GUI::InputField>()->press_button(e.key_code);
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
            if (is_event_logging_active) LOG_INFO("[EVENT] Scroll: {0}x{1}", e.x_offset, e.y_offset);
        });
    m_event_dispather.add_event_listener<EventWindowClose>([&](EventWindowClose& e)
        {
            if (is_event_logging_active) LOG_INFO("[EVENT] Window close");

            m_pCloseWindow = true;
        });
    m_event_dispather.add_event_listener<EventMouseButtonPressed>([&](EventMouseButtonPressed& e)
        {
            m_gui->on_mouse_press(e.x_pos, e.y_pos);
            m_gui_place_menu->on_mouse_press(e.x_pos, e.y_pos);
            m_gui_place_settings->on_mouse_press(e.x_pos, e.y_pos);
            if (is_event_logging_active) LOG_INFO("[EVENT] Mouse button pressed at ({0}x{1})", e.x_pos, e.y_pos);

            Input::pressMouseButton(e.mouse_button);
            m_init_mouse_pos_x = e.x_pos;
            m_init_mouse_pos_y = e.y_pos;
        });
    m_event_dispather.add_event_listener<EventMouseButtonReleased>([&](EventMouseButtonReleased& e)
        {
            isKeyPressedmouse = false;
            m_gui->on_mouse_release(e.x_pos, e.y_pos);
            m_gui_place_menu->on_mouse_release(e.x_pos, e.y_pos);
            m_gui_place_settings->on_mouse_release(e.x_pos, e.y_pos);
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

void GameApp::init_gui()
{
    m_gui = new GUI::GUI_place(m_cam, ResourceManager::getMaterial("default"));
    m_gui_debug = new GUI::GUI_place(m_cam, ResourceManager::getMaterial("default"));
    m_gui_place_menu = new GUI::GUI_place(m_cam, ResourceManager::getMaterial("default"));
    m_gui_place_settings = new GUI::GUI_place(m_cam, ResourceManager::getMaterial("default"));

    // debug window ---------------------------------------------------------------------------
    m_gui_debug->add_element(new GUI::TextRenderer(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "FPS: 0", glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.1f, 98.f), glm::vec2(0.5f), "fps", false));

    m_gui_debug->add_element(new GUI::TextRenderer(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Enemies: 0", glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.1f, 96.f), glm::vec2(0.5f), "enemies", false));

    m_gui_debug->add_element(new GUI::TextRenderer(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Kills: 0", glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.1f, 94.f), glm::vec2(0.5f), "kills", false));        

    // lose window ---------------------------------------------------------------------------
    m_gui->add_element(new GUI::TextRenderer(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "You lose!", glm::vec3(1.f, 0.1f, 0.1f), glm::vec2(41.f, 57.f), glm::vec2(2.f), "Lose text"));

    m_gui->add_element(new GUI::TextRenderer(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Your score: 0", glm::vec3(1.f, 0.1f, 0.1f), glm::vec2(41.f, 50.f), glm::vec2(2.f), "Lose scoreboard"));

    m_gui->add_element(new GUI::Button(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 6.f), glm::vec2(10.f, 5.f),
        "Restart", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f)));

    m_gui->get_element("Restart")->set_click_callback([&]()
        {
            start_game();
        });

    m_gui->set_active(true);
    // Settings ------------------------------------------------------------------------------------    
    m_gui_place_settings->add_element(new GUI::TextRenderer(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Settings", glm::vec3(0.f), glm::vec2(50.f, 90.f), glm::vec2(1.f)));    

    m_gui_place_settings->add_element(new GUI::Button(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 39.f), glm::vec2(10.f, 5.f),
        "Debug", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f)));

    m_gui_place_settings->add_element(new GUI::Button(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 28.f), glm::vec2(10.f, 5.f),
        "Add enemy", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f)));

    m_gui_place_settings->add_element(new GUI::Button(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 17.f), glm::vec2(10.f, 5.f),
        "Grid", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f)));

    m_gui_place_settings->add_element(new GUI::Button(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 6.f), glm::vec2(10.f, 5.f),
        "Back", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f)));

    m_gui_place_settings->add_element(new GUI::Sprite(ResourceManager::getMaterial("defaultSprite"), "default",
        glm::vec2(100.f), glm::vec2(100.f), "z.BG"));
    
    m_gui_place_settings->get_element("Debug")->set_click_callback([&]()
        {
            is_debug_active = !is_debug_active;
        });

    m_gui_place_settings->get_element("Grid")->set_click_callback([&]()
        {
            is_grid_active = !is_grid_active;
        });

    m_gui_place_settings->get_element("Add enemy")->set_click_callback([&]()
        {
            countEnemies++; countEnemiesPerm++;
            m_gui_debug->get_element("enemies")->lead<GUI::TextRenderer>()->set_text("Enemies: " + std::to_string(countEnemiesPerm));
        });

    m_gui_place_settings->get_element("Back")->set_click_callback([&]()
        {
            gui_window = GUI_Active::main;
            m_gui_place_settings->set_active(false);
            m_gui_place_menu->set_active(true);
        });
    // main menu ------------------------------------------------------------------------------------
    m_gui_place_menu->add_element(new GUI::TextRenderer(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Main menu", glm::vec3(0.f), glm::vec2(50.f, 90.f), glm::vec2(1.f)));

    m_gui_place_menu->add_element(new GUI::Button(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(11.f, 6.f), glm::vec2(10.f, 5.f),
        "Quit", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f)));

    m_gui_place_menu->add_element(new GUI::Button(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 6.f), glm::vec2(10.f, 5.f),
        "Settings", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f))); 
    
    m_gui_place_menu->add_element(new GUI::Button(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
            glm::vec2(11.f, 17.f), glm::vec2(10.f, 5.f),
            "Restart", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f)));

    m_gui_place_menu->add_element(new GUI::InputField(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(11.f, 60.f), glm::vec2(10.f, 5.f), "InputTest", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f)));

    m_gui_place_menu->add_element(new GUI::CheckBox(
        new GUI::Sprite(ResourceManager::getMaterial("checkbox_bg")), new GUI::Sprite(ResourceManager::getMaterial("checkbox_mark")),
        glm::vec2(11.f, 50.f), glm::vec2(5.f), "checkbox"));

    m_gui_place_menu->add_element(new GUI::Sprite(ResourceManager::getMaterial("defaultSprite"), "default",
        glm::vec2(100.f), glm::vec2(100.f), "z.BG")); // Crutch but idk how resolve this now

    m_gui_place_menu->get_element("InputTest")->lead<GUI::InputField>()->set_enter_callback([&](std::string text)
        {
            LOG_INFO("Text is: {0}", text);
        });

    m_gui_place_menu->get_element("Quit")->set_click_callback([&]()
        {
            m_pCloseWindow = true;
        });

    m_gui_place_menu->get_element("Settings")->set_click_callback([&]()
        {
            gui_window = GUI_Active::settings;
            m_gui_place_menu->set_active(false);
            m_gui_place_settings->set_active(true);
        });
    m_gui_place_menu->get_element("Restart")->set_click_callback([&]()
        {
            start_game();
            is_gui_active = false;
            m_gui_place_menu->set_active(false);
        });
}

void GameApp::start_game()
{
    map.fill(false);
    isKeyPressed = false;
    m_isLose = false;
    countKills = 0;
    countEnemies = 0; 
    countEnemiesPerm = 0;
    cur = 0;
    gui_window = null;

    m_main_castle = new Castle(parts[int((size_x * size_y) / 2) + int(size_x / 2)], 100,
        "res/models/castle.obj", ResourceManager::getMaterial("castle"), ResourceManager::getMaterial("default"));
    map[int((size_x * size_y) / 2) + int(size_x / 2)] = true;

    m_towers.clear();
    m_enemies.clear();

    m_gui->set_active(false);
    m_gui_debug->set_active(true);
    m_gui_place_menu->set_active(false);
    m_gui_place_settings->set_active(false);

    m_gui_debug->get_element("enemies")->lead<GUI::TextRenderer>()->set_text("Enemies: 0");
    m_gui_debug->get_element("kills")->lead<GUI::TextRenderer>()->set_text("Kills: 0");
}
