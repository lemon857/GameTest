#include "Games/Tower_Defense/GameApplication.h"

#include "EngineCore/System/Log.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Components/Highlight.h"
#include "EngineCore/Components/SpriteRenderer.h"
#include "EngineCore/Components/MeshRenderer.h"
#include "EngineCore/Light/DirectionalLight.h"
#include "EngineCore/Light/PointerLight.h"
#include "EngineCore/System/ShadersSettings.h"

#include "EngineCore/Engine/Window.h"

#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/System/Input.h"

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
#include "EngineCore/GUI/ChatBox.h"
#include "EngineCore/GUI/ScrollBox.h"
#include "EngineCore/GUI/Font.h"

#include "EngineCore/Network/WinSock.h"

#include "EngineCore/System/sysfunc.h"

#include <array>
#include <memory>

//#define CHECK_AVAILABLE_POS(x, y, width, height) ((x > 0 && x < width) && (y > 0 && y < height))

GameApp::GameApp()
    : Application()
    , m_nickname("")
    , m_nickname_connect("")
{
}
GameApp::~GameApp()
{
    delete m_cam;
    WinSock::close_WinSock();
    chat_last.clear();
}

bool GameApp::init()
{
    WinSock::init_WinSock();

    m_cam = new Camera(glm::vec3(0), glm::vec3(0));

    m_cam->set_viewport_size(static_cast<float>(m_pWindow->get_size().x), static_cast<float>(m_pWindow->get_size().y));

    ((float*)ResourceManager::getMaterial("cube")->get_data("ambient_factor"))[0] = 0.25f;
    ((float*)ResourceManager::getMaterial("cube")->get_data("diffuse_factor"))[0] = 0.1f;
    ((float*)ResourceManager::getMaterial("cube")->get_data("specular_factor"))[0] = 0.0f;
    ((float*)ResourceManager::getMaterial("cube")->get_data("metalic_factor"))[0] = 0.0f;

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
    m_scene.add_object<EmptyObject>();

    m_scene.at(curObj)->addComponent<Transform>();
    m_scene.at(curObj)->addComponent<Highlight>(ResourceManager::getMaterial("default"), true);

    m_scene.at(4)->addComponent<Transform>();
    m_scene.at(4)->addComponent<Highlight>(ResourceManager::getMaterial("default"), true, false, glm::vec3(1.f, 0.f, 0.f));
    m_scene.at(4)->getComponent<Highlight>()->set_active(false);

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
    init_winSock();

    m_gui->set_active(false);
    m_gui_chat->set_active(true);
    m_gui_debug->set_active(true);
    m_gui_place_menu->set_active(false);
    m_gui_place_settings->set_active(false);

    m_gui_place_menu->get_element<GUI::Button>("Disconnect")->set_active(false);

    m_gui_chat->get_element<GUI::ChatBox>("Chat")->set_open(false);
    m_gui_chat->get_element<GUI::InputField>("SendMessage")->set_active(false);

    m_scene.at(curObj)->getComponent<Transform>()->set_position(parts[0]);

    return true;
}

void GameApp::on_key_update(const double delta)
{
    if (m_isLose
        || m_gui_place_menu->get_element<GUI::InputField>("InputIP")->get_focus()
        || m_gui_place_menu->get_element<GUI::InputField>("InputNick")->get_focus()
        || m_gui_chat->get_element<GUI::InputField>("SendMessage")->get_focus())
    {
        if (Input::isKeyPressed(KeyCode::KEY_ESCAPE))
        {
            m_gui_chat->get_element<GUI::InputField>("SendMessage")->set_focus(false);
            isKeyPressed = true;
            if (is_chat_active)
            {
                m_gui_chat->get_element<GUI::ChatBox>("Chat")->set_open(false);
                m_gui_chat->get_element<GUI::InputField>("SendMessage")->set_active(false);
                is_chat_active = false;
            }
        }
        if (m_gui_chat->get_element<GUI::InputField>("SendMessage")->get_focus())
        {
            if (Input::isKeyPressed(KeyCode::KEY_UP) && !isKeyPressed)
            {
                if (cur_chat_index < chat_last.size() - 1) cur_chat_index++;
                if (!chat_last.empty()) m_gui_chat->get_element<GUI::InputField>("SendMessage")->set_text(chat_last[cur_chat_index]);
                isKeyPressed = true;
            }
            else if (Input::isKeyPressed(KeyCode::KEY_DOWN) && !isKeyPressed)
            {
                if (cur_chat_index > 0) cur_chat_index--;
                if (!chat_last.empty()) m_gui_chat->get_element<GUI::InputField>("SendMessage")->set_text(chat_last[cur_chat_index]);
                isKeyPressed = true;
            }
        }
        return;
    }

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
    else if (Input::isKeyPressed(KeyCode::KEY_T))
    {
        if (is_chat_active && !isKeyPressed && !is_chat_full_hide)
        {
            m_gui_chat->get_element<GUI::ChatBox>("Chat")->set_open(false);
            m_gui_chat->get_element<GUI::InputField>("SendMessage")->set_active(false);
            is_chat_active = false;
            isKeyPressed = true;
        }
        else if (!isKeyPressed && !is_chat_full_hide)
        {
            m_gui_chat->get_element<GUI::InputField>("SendMessage")->set_focus(true);
            m_gui_chat->get_element<GUI::InputField>("SendMessage")->set_active(true);
            m_gui_chat->get_element<GUI::ChatBox>("Chat")->set_open(true);
            m_gui_chat->get_element<GUI::InputField>("SendMessage")->set_text("");
            is_chat_active = true;
            isKeyPressed = true;
        }
    }
    else if (Input::isKeyPressed(KeyCode::KEY_M))
    {
        if (is_spawn_enemy && !isKeyPressed)
        {
            if (is_spawn_mode)
            {
                m_scene.at(curObj)->getComponent<Highlight>()->set_color(glm::vec3(1.f, 1.f, 0.f));
            }
            else
            {
                m_scene.at(curObj)->getComponent<Highlight>()->set_color(glm::vec3(1.f));
            }

            is_spawn_enemy = false;
            isKeyPressed = true;
        }
        else if (!isKeyPressed)
        {
            if (is_spawn_mode)
            {
                m_scene.at(curObj)->getComponent<Highlight>()->set_color(glm::vec3(0.f, 1.f, 1.f));
            }
            else
            {
                m_scene.at(curObj)->getComponent<Highlight>()->set_color(glm::vec3(1.f, 0.f, 1.f));
            }
            is_spawn_enemy = true;
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
            short snum = rand() % 4;
            unsigned int spawn = 0;
            if (snum == 0) spawn = rand() % size_x;
            else if (snum == 1) spawn = (rand() % size_x) + ((size_x - 1) * size_y);
            else if (snum == 2) spawn = (rand() % size_y) * size_x;
            else if (snum == 3) spawn = ((rand() % size_y) * size_x) + (size_x - 1);
            m_spawn_enemies.push(spawn);

            char buff[sizeof(unsigned int) + 1];
            buff[0] = 's';
            sysfunc::type_to_char(&spawn, buff, 1);
            WinSock::send_data(buff, sizeof(unsigned int) + 1);

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
    else if (Input::isKeyPressed(KeyCode::KEY_F1))
    {
        if (is_chat_full_hide && !isKeyPressed)
        {
            is_chat_full_hide = false;
            isKeyPressed = true;
        }
        else if (!isKeyPressed)
        {
            is_chat_full_hide = true;
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

    if (is_gui_active) return; // block move in menu

    if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT) && !m_gui_place_menu->get_focus())
    {
        int x = (int)floor(m_world_mouse_pos_x / 2.f);
        int y = (int)floor(m_world_mouse_pos_z / 2.f);

        if ((x * y <= (size_x - 1) * (size_y - 1)) && (x >= 0 && y >= 0))
        {
            if (x <= (size_x - 1) && y <= (size_y - 1))
            {
                if (cur != x * size_y + y)
                {
                    cur = x * size_y + y;
                    m_scene.at(curObj)->getComponent<Transform>()->set_position(parts[cur]);

                    isKeyPressedmouse = true;

                    if (is_spawn_mode)
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
                            isKeyPressedmouse = true;

                            char buff[sizeof(unsigned int) + 1];

                            if (is_spawn_enemy)
                            {
                                m_spawn_enemies.push(cur);
                                buff[0] = 's';
                                sysfunc::type_to_char(&cur, buff, 1);

                                WinSock::send_data(buff, sizeof(unsigned int) + 1);
                            }
                            else if (!map[cur])
                            {
                                map[cur] = true;
                                m_spawn_towers.push(cur);
                                buff[0] = 't';
                                sysfunc::type_to_char(&cur, buff, 1);

                                WinSock::send_data(buff, sizeof(unsigned int) + 1);
                                LOG_INFO("Add tower at {0}x{1}", x, y);
                            }


                            //m_scene.at(curObj)->deleteComponent<Highlight>();
                            //curObj++;
                            // 
                            //m_scene.add_object<ObjModel>("res/models/monkey.obj", ResourceManajger::getMaterial("monkey"));
                            //m_scene.add_object<Cube>(ResourceManager::getMaterial("cube"));

                            //m_scene.at(curObj)->addComponent<Transform>();
                            //m_scene.at(curObj)->addComponent<Highlight>(ResourceManager::getMaterial("default"), true);
                        }
                    }

                    char buff[sizeof(unsigned int) + 1];
                    buff[0] = 'd';
                    sysfunc::type_to_char(&cur, buff, 1);
                    WinSock::send_data(buff, sizeof(unsigned int) + 1);
                }
                else if (!is_spawn_mode && !isKeyPressedmouse)
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
                        isKeyPressedmouse = true;

                        char buff[sizeof(unsigned int) + 1];

                        if (is_spawn_enemy)
                        {
                            m_spawn_enemies.push(cur);
                            buff[0] = 's';
                            sysfunc::type_to_char(&cur, buff, 1);

                            WinSock::send_data(buff, sizeof(unsigned int) + 1);
                        }
                        else if (!map[cur])
                        {
                            map[cur] = true;
                            m_spawn_towers.push(cur);
                            buff[0] = 't';
                            sysfunc::type_to_char(&cur, buff, 1);

                            WinSock::send_data(buff, sizeof(unsigned int) + 1);
                            LOG_INFO("Add tower at {0}x{1}", x, y);
                        }



                        //m_scene.at(curObj)->deleteComponent<Highlight>();
                        //curObj++;
                        // 
                        //m_scene.add_object<ObjModel>("res/models/monkey.obj", ResourceManajger::getMaterial("monkey"));
                        //m_scene.add_object<Cube>(ResourceManager::getMaterial("cube"));

                        //m_scene.at(curObj)->addComponent<Transform>();
                        //m_scene.at(curObj)->addComponent<Highlight>(ResourceManager::getMaterial("default"), true);
                    }
                }
            }
        }
    }
    else if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_RIGHT))
    {
        if (is_spawn_mode && !isKeyPressedmouse)
        {
            if (is_spawn_enemy)
            {
                m_scene.at(curObj)->getComponent<Highlight>()->set_color(glm::vec3(1.f, 0.f, 1.f));
            }
            else
            {
                m_scene.at(curObj)->getComponent<Highlight>()->set_color(glm::vec3(1.f));
            }
            is_spawn_mode = false;
            isKeyPressedmouse = true;
        }
        else if (!isKeyPressedmouse)
        {
            if (is_spawn_enemy)
            {
                m_scene.at(curObj)->getComponent<Highlight>()->set_color(glm::vec3(1.f, 0.f, 1.f));
            }
            else
            {
                m_scene.at(curObj)->getComponent<Highlight>()->set_color(glm::vec3(0.f, 1.f, 1.f));
            }
            is_spawn_mode = true;
            isKeyPressedmouse = true;
        }
    }
    // movement
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

    m_scene.at(4)->getComponent<Transform>()->set_position(parts[cur_player]);
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
    // ============================================================
    if (m_isLose) return;

    m_main_castle->update(delta);

    if (m_main_castle->isDestroyed() && !m_isLose)
    {
        m_isLose = true;
        m_gui_place_menu->set_active(false);
        m_gui_place_settings->set_active(false);
        gui_window = null;
        m_gui->get_element<GUI::TextRenderer>("Lose scoreboard")->set_text("Your score: " + std::to_string(countKills));
        m_gui->set_active(true);
        //if (isServer) WinSock::send_data("l", 1);
    }
    // sync

    while (!m_spawn_towers.empty())
    {
        m_towers.push_back(new BaseTower("res/models/tower.obj",
            ResourceManager::getMaterial("tower"), nullptr, parts[m_spawn_towers.front()], _set_cooldown_tower, _set_damage_tower, new RenderEngine::Line(ResourceManager::getMaterial("default"))));
        m_spawn_towers.pop();
    }
    while (!m_spawn_enemies.empty()) // spawn enemies
    {
        countEnemiesPerm++;
        auto a = new BaseEnemy(new ObjModel("res/models/monkey.obj", ResourceManager::getMaterial("monkey")),
            m_main_castle, parts[m_spawn_enemies.front()], 1, _set_velosity * 0.001, _set_max_hp_enemy, ResourceManager::getMaterial("default"));
        m_enemies.push_back(a);
        m_spawn_enemies.pop();
        m_gui_debug->get_element<GUI::TextRenderer>("enemies")->set_text("Enemies: " + std::to_string(countEnemiesPerm));
    }

    for (auto curTower : m_towers)
    {
        double distance = 100;
        for (size_t i = 0; i < m_enemies.size(); i++)
        {
            if (m_enemies[i] == nullptr) continue;
            glm::vec3 a = m_enemies[i]->get_pos() - curTower->get_pos();
            double d = sqrt(a.x * a.x + a.z * a.z);
            if (d < _set_min_distance && d < distance)
            {
                distance = d;
                if (curTower->get_target() == nullptr) curTower->set_target(m_enemies[i]);
                //if (curTower->get_target() != m_enemies[i]) curTower->set_target(m_enemies[i]);
            }
            else 
            {
                //curTower->set_target(nullptr);
            }
        }
        curTower->update(delta);
    }

    for (size_t i = 0; i < m_enemies.size(); i++)
    {
        if (m_enemies[i] == nullptr) continue;        
        if (m_enemies[i]->is_destroy())
        {
            //countEnemies++;
            countEnemiesPerm--;
            for (auto curTower : m_towers)
            {
                if (curTower->get_target() == m_enemies[i]) curTower->set_target(nullptr);
            }
            delete m_enemies[i];
            m_enemies.remove(i);
            countKills++;
            m_gui_debug->get_element<GUI::TextRenderer>("kills")->set_text("Kills: " + std::to_string(countKills));
            m_gui_debug->get_element<GUI::TextRenderer>("enemies")->set_text("Enemies: " + std::to_string(countEnemiesPerm));
            continue;
        }
        m_enemies[i]->update(delta);        
    }
    // fps counter
    if (frames < 5)
    {
        frames++;
        times += delta;
    }
    else
    {
        fps = int((frames / times) * 1000.f);
        m_gui_debug->get_element<GUI::TextRenderer>("fps")->set_text("FPS: " + std::to_string(fps));
        frames = 0;
        times = 0;
    }
}

void GameApp::on_ui_render()
{
    // for correct write message in other thread
    while (!m_chat_mes.empty())
    {
        m_gui_chat->get_element<GUI::ChatBox>("Chat")->add_message(m_chat_mes.front());
        LOG_INFO("[CHAT] {0}", m_chat_mes.front());
        m_chat_mes.pop();
    }
    // =========================================
    m_gui->on_render();
    if (!is_chat_full_hide) m_gui_chat->on_render();
    if (is_gui_active)
    {
        if (gui_window == main) m_gui_place_menu->on_render();
        else if (gui_window == settings) m_gui_place_settings->on_render();
        return;
    }
    if (is_debug_active) m_gui_debug->on_render();
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
                m_gui_chat->on_resize();
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
            m_gui_place_menu->get_element<GUI::InputField>("InputNick")->press_button(e.key_code);
            m_gui_place_menu->get_element<GUI::InputField>("InputIP")->press_button(e.key_code);
            m_gui_chat->get_element<GUI::InputField>("SendMessage")->press_button(e.key_code);
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
    m_event_dispather.add_event_listener<EventMouseScrolled>([&](EventMouseScrolled& e)
        {
            if (is_chat_active) m_gui_chat->get_element<GUI::ChatBox>("Chat")->on_scroll(-e.y_offset);
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
            m_gui_chat->on_mouse_press(e.x_pos, e.y_pos);
            m_gui_debug->on_mouse_press(e.x_pos, e.y_pos);
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
            m_gui_debug->on_mouse_release(e.x_pos, e.y_pos);
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
    ResourceManager::get_font("calibri")->set_scale(0.5f);
    ResourceManager::get_font("calibriChat")->set_scale(0.3f);

    m_gui = new GUI::GUI_place(m_cam, ResourceManager::getMaterial("default"));
    m_gui_chat = new GUI::GUI_place(m_cam, ResourceManager::getMaterial("default"));
    m_gui_debug = new GUI::GUI_place(m_cam, ResourceManager::getMaterial("default"));
    m_gui_place_menu = new GUI::GUI_place(m_cam, ResourceManager::getMaterial("default"));
    m_gui_place_settings = new GUI::GUI_place(m_cam, ResourceManager::getMaterial("default"));

    // debug window ---------------------------------------------------------------------------
    float offset = 98.f;

    m_gui_debug->add_element<GUI::TextRenderer>(ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"),
        BUILD_NAME, glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.1f, offset), glm::vec2(0.5f), "version_build", false);
    offset -= 2.5f;

    m_gui_debug->add_element<GUI::TextRenderer>(ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"),
        "FPS: 0", glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.1f, offset), glm::vec2(0.5f), "fps", false);
    offset -= 2.5f;

    m_gui_debug->add_element<GUI::TextRenderer>(ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"),
        "Enemies: 0", glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.1f, offset), glm::vec2(0.5f), "enemies", false);
    offset -= 2.5f;

    m_gui_debug->add_element<GUI::TextRenderer>(ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"),
        "Kills: 0", glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.1f, offset), glm::vec2(0.5f), "kills", false);
    offset -= 2.5f;

    m_gui_debug->add_element<GUI::TextRenderer>(ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"),
        "Ping: 0", glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.1f, offset), glm::vec2(0.5f), "ping", false);

    // chat ---------------------------------------------------------------------------
    m_gui_chat->add_element<GUI::ChatBox>(new GUI::Sprite(ResourceManager::getMaterial("defaultSprite")),
        glm::vec2(13.f, 36.f), glm::vec2(12.f, 30.f), "Chat", 128, ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"), glm::vec3(1.f));

    m_gui_chat->add_element<GUI::InputField>(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(13.f, 3.f), glm::vec2(12.f, 3.f), "SendMessage", ResourceManager::getShaderProgram("textShader"),
        ResourceManager::get_font("calibriChat"), glm::vec3(1.f), true)->set_enter_callback([&](std::string text) {
            if (text[0] == '/') // need finalize
            {
                char buff[sizeof(double) + 2];
                buff[0] = 'c';

                if (ResourceManager::start_with(text, "/dis")) // distance tower attack a
                {
                    if (text == "/dis")
                    {
                        m_chat_mes.push("Min distance: " + std::to_string(_set_min_distance));
                    }
                    else
                    {
                        _set_min_distance = std::stod(text.substr(5));
                        m_chat_mes.push("Set min distance: " + std::to_string(_set_min_distance));
                        buff[1] = 'a';
                        sysfunc::type_to_char(&_set_min_distance, buff, 2);
                        WinSock::send_data(buff, sizeof(double) + 2);
                    }
                }
                else if (ResourceManager::start_with(text, "/vel")) // velosity enemy s
                {
                    if (text == "/vel")
                    {
                        m_chat_mes.push("Velocity: " + std::to_string(_set_velosity));             
                    }
                    else
                    {
                        _set_velosity = std::stod(text.substr(5));
                        m_chat_mes.push("Set velocity: " + std::to_string(_set_velosity));
                        buff[1] = 's';
                        sysfunc::type_to_char(&_set_velosity, buff, 2);
                        WinSock::send_data(buff, sizeof(double) + 2);
                    }
                }
                else if (ResourceManager::start_with(text, "/mhpcas")) // max hp castle d
                {
                    if (text == "/mhpcas")
                    {
                        m_chat_mes.push("Max hp castle: " + std::to_string(_set_max_hp_castle));
                    }
                    else
                    {
                        _set_max_hp_castle = std::stod(text.substr(8));
                        m_chat_mes.push("Set max hp castle: " + std::to_string(_set_max_hp_castle));
                        m_chat_mes.push("Need restart");
                        buff[1] = 'd';
                        sysfunc::type_to_char(&_set_max_hp_castle, buff, 2);
                        WinSock::send_data(buff, sizeof(double) + 2);
                    }
                }
                else if (ResourceManager::start_with(text, "/mhpen")) // max hp enemy f
                {
                    if (text == "/mhpen")
                    {
                        m_chat_mes.push("Max hp enemy: " + std::to_string(_set_max_hp_enemy));
                    }
                    else
                    {
                        _set_max_hp_enemy = std::stod(text.substr(7));
                        m_chat_mes.push("Set max hp enemy: " + std::to_string(_set_max_hp_enemy));
                        buff[1] = 'f';
                        sysfunc::type_to_char(&_set_max_hp_enemy, buff, 2);
                        WinSock::send_data(buff, sizeof(double) + 2);
                    }
                }
                else if (ResourceManager::start_with(text, "/cdtow")) // cooldown tower attack g
                {
                    if (text == "/cdtow")
                    {
                        m_chat_mes.push("Cooldown tower: " + std::to_string(_set_cooldown_tower));
                    }
                    else
                    {
                        _set_cooldown_tower = std::stod(text.substr(7));
                        m_chat_mes.push("Set cooldown tower: " + std::to_string(_set_cooldown_tower));
                        m_chat_mes.push("Need restart");
                        buff[1] = 'g';
                        sysfunc::type_to_char(&_set_cooldown_tower, buff, 2);
                        WinSock::send_data(buff, sizeof(double) + 2);
                    }
                }
                else if (ResourceManager::start_with(text, "/dmgtow")) // damage tower attack h
                {
                    if (text == "/dmgtow")
                    {
                        m_chat_mes.push("Damage tower: " + std::to_string(_set_damage_tower));
                    }
                    else
                    {
                        char buff1[sizeof(unsigned int) + 2];
                        buff1[0] = 'c';
                        _set_cooldown_tower = std::stod(text.substr(8));
                        m_chat_mes.push("Set damage tower: " + std::to_string(_set_damage_tower));
                        m_chat_mes.push("Need restart");
                        buff1[1] = 'h';
                        sysfunc::type_to_char(&_set_damage_tower, buff1, 2);
                        WinSock::send_data(buff1, sizeof(unsigned int) + 2);
                    }
                }
            }
            else
            {
                m_chat_mes.push(m_nickname + ": " + text);
                WinSock::send_data(('m' + text).data(), text.length() + 1); // =========================== message flag first
            }
            chat_last.push_back(text);
            });
    
    //m_gui_chat->add_element<GUI::ScrollBox>(new GUI::Sprite(ResourceManager::getMaterial("defaultSprite")),
     //   glm::vec2(40.f, 41.f), glm::vec2(20.f, 30.f), "Scroll", 10);
   // m_gui_chat->get_element<GUI::ScrollBox>("Scroll")->add_element(new GUI::TextRenderer(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
    //    "Settings", glm::vec3(0.f), glm::vec2(50.f, 90.f), glm::vec2(1.f)));
   // m_gui_chat->get_element<GUI::ScrollBox>("Scroll")->add_element(new GUI::TextRenderer(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
    //    "Settings", glm::vec3(0.f), glm::vec2(50.f, 90.f), glm::vec2(1.f)));

    // lose window ---------------------------------------------------------------------------
    m_gui->add_element<GUI::TextRenderer>(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "You lose!", glm::vec3(1.f, 0.1f, 0.1f), glm::vec2(41.f, 57.f), glm::vec2(2.f), "Lose text");

    m_gui->add_element<GUI::TextRenderer>(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Your score: 0", glm::vec3(1.f, 0.1f, 0.1f), glm::vec2(41.f, 50.f), glm::vec2(2.f), "Lose scoreboard");

    m_gui->add_element<GUI::Button>(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 6.f), glm::vec2(10.f, 5.f),
        "Restart", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f));

    m_gui->get_element<GUI::GUI_element>("Restart")->set_click_callback([&]()
        {
            if (isServer) WinSock::send_data("r", 1);
            start_game();
        });

    m_gui->set_active(true);
    // Settings ------------------------------------------------------------------------------------    
    m_gui_place_settings->add_element<GUI::TextRenderer>(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Settings", glm::vec3(1.f), glm::vec2(50.f, 90.f), glm::vec2(1.f));

    m_gui_place_settings->add_element<GUI::Button>(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 39.f), glm::vec2(10.f, 5.f),
        "Clear chat", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                m_gui_chat->get_element<GUI::ChatBox>("Chat")->clear();
            });

    m_gui_place_settings->add_element<GUI::Button>(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 28.f), glm::vec2(10.f, 5.f),
        "Add enemy", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                short snum = rand() % 4;
                int spawn = 0;
                if (snum == 0) spawn = rand() % size_x;
                else if (snum == 1) spawn = (rand() % size_x) + ((size_x - 1) * size_y);
                else if (snum == 2) spawn = (rand() % size_y) * size_x;
                else if (snum == 3) spawn = ((rand() % size_y) * size_x) + (size_x - 1);
                m_spawn_enemies.push(spawn);

                char buff[sizeof(unsigned int) + 1];
                buff[0] = 's';
                sysfunc::type_to_char(&spawn, buff, 1);
                WinSock::send_data(buff, sizeof(unsigned int) + 1);          
            });;

    m_gui_place_settings->add_element<GUI::Button>(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 17.f), glm::vec2(10.f, 5.f),
        "Grid", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                is_grid_active = !is_grid_active;
            });

    m_gui_place_settings->add_element<GUI::Button>(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 6.f), glm::vec2(10.f, 5.f),
        "Back", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                gui_window = GUI_Active::main;
                m_gui_place_settings->set_active(false);
                m_gui_place_menu->set_active(true);
            });

    m_gui_place_settings->add_element<GUI::Sprite>(ResourceManager::getMaterial("defaultSprite"), "default",
        glm::vec2(100.f), glm::vec2(100.f), "z.BG");
    
    // main menu ------------------------------------------------------------------------------------
    m_gui_place_menu->add_element<GUI::TextRenderer>(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Main menu", glm::vec3(1.f), glm::vec2(50.f, 90.f), glm::vec2(1.f));

    m_gui_place_menu->add_element<GUI::Button>(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 6.f), glm::vec2(10.f, 5.f),
        "Quit", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                m_pCloseWindow = true;
            });;

    m_gui_place_menu->add_element<GUI::Button>(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 28.f), glm::vec2(10.f, 5.f),
        "Settings", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                gui_window = GUI_Active::settings;
                m_gui_place_menu->set_active(false);
                m_gui_place_settings->set_active(true);
            });;
    
    m_gui_place_menu->add_element<GUI::Button>(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
            glm::vec2(89.f, 17.f), glm::vec2(10.f, 5.f),
            "Restart", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
                {
                    if (isServer) WinSock::send_data("r", 1);
                    start_game();
                    is_gui_active = false;
                    m_gui_place_menu->set_active(false);
                });;   

    // ========================================================================================
    m_gui_place_menu->add_element<GUI::TextRenderer>(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Enter Nickname", glm::vec3(1.f), glm::vec2(88.f, 88.f), glm::vec2(1.f), "m1"); 

    m_gui_place_menu->add_element<GUI::InputField>(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(88.f, 80.f), glm::vec2(10.f, 5.f), "InputNick", ResourceManager::getShaderProgram("textShader"),
        ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_enter_callback([&](std::string nick)
            {
                if (nick.empty())
                {
                    m_gui_place_menu->get_element<GUI::TextRenderer>("m1")->set_text("Fill nickname");
                }
                else
                {
                    m_nickname = nick;
                }
            });

    m_gui_place_menu->add_element<GUI::TextRenderer>(ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Enter IP", glm::vec3(1.f), glm::vec2(88.f, 68.f), glm::vec2(1.f), "m");

    m_gui_place_menu->add_element<GUI::InputField>(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(88.f, 60.f), glm::vec2(10.f, 5.f), "InputIP", ResourceManager::getShaderProgram("textShader"),
        ResourceManager::get_font("calibri"), glm::vec3(1.f));

    m_gui_place_menu->add_element<GUI::Button>(new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(88.f, 48.f), glm::vec2(10.f, 5.f),
        "Disconnect", "textShader", ResourceManager::get_font("calibri"), glm::vec3(1.f));

    m_gui_place_menu->add_element<GUI::CheckBox>(new GUI::Sprite(ResourceManager::getMaterial("checkbox_bg")), new GUI::Sprite(ResourceManager::getMaterial("checkbox_mark")),
        glm::vec2(70.f, 60.f), glm::vec2(5.f), "checkbox")->set_click_callback([&]() {
            if (m_gui_place_menu->get_element<GUI::CheckBox>("checkbox")->value())
            {
                m_gui_place_menu->get_element<GUI::InputField>("InputIP")->set_text("0.0.0.0");
            }
            else
            {
                m_gui_place_menu->get_element<GUI::InputField>("InputIP")->set_text("127.0.0.1");
            }
            });

    m_gui_place_menu->add_element<GUI::Sprite>(ResourceManager::getMaterial("defaultSprite"), "default",
        glm::vec2(100.f), glm::vec2(100.f), "z.BG"); // Crutch but idk how resolve this now    

    m_gui_place_menu->get_element<GUI::GUI_element>("Disconnect")->set_click_callback([&]() {
        WinSock::disconnect();
        m_chat_mes.push("Disconnect!");
        });

    m_gui_place_menu->get_element<GUI::InputField>("InputIP")->set_text("127.0.0.1");

    m_gui_place_menu->get_element<GUI::InputField>("InputIP")->set_enter_callback([&](std::string text)
        {
            if (m_nickname.empty())
            {
                m_chat_mes.push("Enter nickname!");
                return;
            }
            //m_gui_chat->get_element<GUI::ChatBox>("Chat")->clear(); 
            WinSock::InitCodes code;
            if (m_gui_place_menu->get_element<GUI::CheckBox>("checkbox")->value())
            {
                code = (WinSock::InitCodes)WinSock::open_server(text.c_str(), 20746);
                isServer = true;
            }
            else
            {
                code = (WinSock::InitCodes)WinSock::open_client(text.c_str(), 20746);
                isServer = false;
            }
            switch (code)
            {
            case WinSock::OK:
                m_chat_mes.push("Wait connection!");
                break;
            case WinSock::IP_ERR:
                m_chat_mes.push("Uncorrect IP!");
                break;
            case WinSock::INIT_SOCK_ERR:
                m_chat_mes.push("Init sock error!");
                break;
            case WinSock::CONNECT_ERR:
                m_chat_mes.push("Connect error!");
                break;
            }
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

    m_main_castle = new Castle(parts[int((size_x * size_y) / 2) + int(size_x / 2)], _set_max_hp_castle,
        "res/models/castle.obj", ResourceManager::getMaterial("castle"), ResourceManager::getMaterial("default"));
    map[int((size_x * size_y) / 2) + int(size_x / 2)] = true;

    m_towers.clear();
    m_enemies.clear();
    
    is_gui_active = false;

    is_spawn_mode = false;
    is_spawn_enemy = false;
    m_scene.at(curObj)->getComponent<Highlight>()->set_color(glm::vec3(1.f));

    m_gui_debug->get_element<GUI::TextRenderer>("enemies")->set_text("Enemies: 0");
    m_gui_debug->get_element<GUI::TextRenderer>("kills")->set_text("Kills: 0");

    m_gui->set_active(false);

    // crutch, but now is the best solve (reset settings when connect)
    //_set_min_distance = 7;
    //_set_velosity = 7;
    //_set_max_hp_castle = 100;
    //_set_max_hp_enemy = 50;
    //_set_cooldown_tower = 3;
    //_set_damage_tower = 10;
}

void GameApp::init_winSock()
{
    WinSock::set_receive_callback([&](char* data, int size) {
        if (data[WS_DATA_PACKET_INFO_SIZE] == 'm') // message for chat
        {
            std::string str = std::string(&data[WS_DATA_PACKET_INFO_SIZE + 1]).substr(0, size - (WS_DATA_PACKET_INFO_SIZE));
            m_chat_mes.push(m_nickname_connect + ": " + str);
        }
        else if (data[WS_DATA_PACKET_INFO_SIZE] == 'q') // kill enemy (not working now)
        {
            unsigned int dmg = 0;
            sysfunc::char_to_type(&dmg, data, WS_DATA_PACKET_INFO_SIZE + 1);
            //m_main_castle->damage(dmg);
        }
        else if (data[WS_DATA_PACKET_INFO_SIZE] == 'w') // damage castle (not working now)
        {
            unsigned int dmg = 0;
            sysfunc::char_to_type(&dmg, data, WS_DATA_PACKET_INFO_SIZE + 1);
            m_main_castle->damage(dmg);
        }
        else if (data[WS_DATA_PACKET_INFO_SIZE] == 'l') // lose
        {
            m_main_castle->damage(_set_max_hp_castle + 1);
        }
        else if (data[WS_DATA_PACKET_INFO_SIZE] == 'r') // restart
        {
            start_game();
        }
        else if (data[WS_DATA_PACKET_INFO_SIZE] == 't') // spawn tower
        {
            unsigned int cur_buf;
            sysfunc::char_to_type(&cur_buf, data, WS_DATA_PACKET_INFO_SIZE + 1);
            m_spawn_towers.push(cur_buf);
        }
        else if (data[WS_DATA_PACKET_INFO_SIZE] == 's') // spawn enemy
        {
            unsigned int spawn = 0;
            sysfunc::char_to_type(&spawn, data, WS_DATA_PACKET_INFO_SIZE + 1);
            m_spawn_enemies.push(spawn);
        }
        else if (data[WS_DATA_PACKET_INFO_SIZE] == 'd') // cursor move
        {
            sysfunc::char_to_type(&cur_player, data, WS_DATA_PACKET_INFO_SIZE + 1);
        }
        else if (data[WS_DATA_PACKET_INFO_SIZE] == 'n')
        {
            std::string str = std::string(&data[WS_DATA_PACKET_INFO_SIZE + 1]).substr(0, size - (WS_DATA_PACKET_INFO_SIZE));
            m_nickname_connect = str;
            std::string temp = isServer ? ": " : "to: ";
            m_chat_mes.push("Connect " + temp + str);
        }
        else if (data[WS_DATA_PACKET_INFO_SIZE] == 'c')
        {
            if (data[WS_DATA_PACKET_INFO_SIZE + 1] == 'a')
            {
                sysfunc::char_to_type(&_set_min_distance, data, WS_DATA_PACKET_INFO_SIZE + 2);
                m_chat_mes.push(m_nickname_connect + ": Set min distance: " + std::to_string(_set_min_distance));
            }
            else if (data[WS_DATA_PACKET_INFO_SIZE + 1] == 's')
            {
                sysfunc::char_to_type(&_set_velosity, data, WS_DATA_PACKET_INFO_SIZE + 2);
                m_chat_mes.push(m_nickname_connect + ": Set velocity: " + std::to_string(_set_velosity));
            }
            else if (data[WS_DATA_PACKET_INFO_SIZE + 1] == 'd')
            {
                sysfunc::char_to_type(&_set_max_hp_castle, data, WS_DATA_PACKET_INFO_SIZE + 2);
                m_chat_mes.push(m_nickname_connect + ": Max hp castle: " + std::to_string(_set_max_hp_castle));
                m_chat_mes.push("Need restart");
            }
            else if (data[WS_DATA_PACKET_INFO_SIZE + 1] == 'f')
            {
                sysfunc::char_to_type(&_set_max_hp_enemy, data, WS_DATA_PACKET_INFO_SIZE + 2);
                m_chat_mes.push(m_nickname_connect + ": Set max hp enemy: " + std::to_string(_set_max_hp_enemy));
            }
            else if (data[WS_DATA_PACKET_INFO_SIZE + 1] == 'g')
            {
                sysfunc::char_to_type(&_set_cooldown_tower, data, WS_DATA_PACKET_INFO_SIZE + 2);
                m_chat_mes.push(m_nickname_connect + ": Set cooldown tower: " + std::to_string(_set_cooldown_tower));
                m_chat_mes.push("Need restart");
            }
            else if (data[WS_DATA_PACKET_INFO_SIZE + 1] == 'h')
            {
                sysfunc::char_to_type(&_set_damage_tower, data, WS_DATA_PACKET_INFO_SIZE + 2);
                m_chat_mes.push(m_nickname_connect + ": Set damage tower: " + std::to_string(_set_damage_tower));
                m_chat_mes.push("Need restart");
            }
        }
        });

    WinSock::set_ping_callback([&](double ping) {
        m_gui_debug->get_element<GUI::TextRenderer>("ping")->set_text(std::to_string((float)ping) + " ms");
        });

    WinSock::set_disconnect_callback([&]() {
        m_chat_mes.push("Disconnect!");
        m_scene.at(4)->getComponent<Highlight>()->set_active(false);
        m_gui_place_menu->get_element<GUI::Button>("Disconnect")->set_active(false);
        m_gui_place_menu->get_element<GUI::Button>("Restart")->set_active(true);
        m_gui->get_element<GUI::Button>("Restart")->set_active(true);
        });
    WinSock::set_connect_callback([&]() {
        WinSock::send_data(('n' + m_nickname).data(), m_nickname.length() + 1); // =========================== message flag first
        m_scene.at(4)->getComponent<Highlight>()->set_active(true);
        m_gui_place_menu->get_element<GUI::Button>("Disconnect")->set_active(true);
        if (!isServer)
        {
            m_gui_place_menu->get_element<GUI::Button>("Restart")->set_active(false);
            m_gui->get_element<GUI::Button>("Restart")->set_active(false);
        }
        else
        {
            char buff[sizeof(double) + 2];
            buff[0] = 'c';

            buff[1] = 'a';
            sysfunc::type_to_char(&_set_min_distance, buff, 2);
            WinSock::send_data(buff, sizeof(double) + 2);

            buff[1] = 's';
            sysfunc::type_to_char(&_set_velosity, buff, 2);
            WinSock::send_data(buff, sizeof(double) + 2);

            buff[1] = 'd';
            sysfunc::type_to_char(&_set_max_hp_castle, buff, 2);
            WinSock::send_data(buff, sizeof(double) + 2);

            buff[1] = 'f';
            sysfunc::type_to_char(&_set_max_hp_enemy, buff, 2);
            WinSock::send_data(buff, sizeof(double) + 2);

            buff[1] = 'g';
            sysfunc::type_to_char(&_set_cooldown_tower, buff, 2);
            WinSock::send_data(buff, sizeof(double) + 2);

            char buff1[sizeof(unsigned int) + 2];
            buff1[0] = 'c';

            buff1[1] = 'h';
            sysfunc::type_to_char(&_set_damage_tower, buff1, 2);
            WinSock::send_data(buff1, sizeof(unsigned int) + 2);
        }
        start_game();
        });
}
