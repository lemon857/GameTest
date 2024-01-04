#include "Games/Tower_Defense/GameApplication.h"

#include "EngineCore/System/Log.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Components/Highlight.h"
#include "EngineCore/Components/SpriteRenderer.h"
#include "EngineCore/Components/MeshRenderer.h"
#include "EngineCore/Light/DirectionalLight.h"
#include "EngineCore/Light/PointerLight.h"
#include "EngineCore/System/ShadersSettings.h"
#include "EngineCore/System/CommandManager.h"

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
#include "EngineCore/GUI/BindButton.h"
#include "EngineCore/GUI/Slider.h"
#include "EngineCore/GUI/Sprite.h"
#include "EngineCore/GUI/InputField.h"
#include "EngineCore/GUI/CheckBox.h"
#include "EngineCore/GUI/ChatBox.h"
#include "EngineCore/GUI/ScrollBox.h"
#include "EngineCore/GUI/Font.h"

#include "EngineCore/Network/WinSock.h"

#include "EngineCore/System/sysfunc.h"

#include "EngineCore/Sound/Sound.h"
#include "EngineCore/Sound/SoundEngine.h"

#include <array>
#include <memory>

#define GET_DATA_MATERIAL(name_material, type, name_data, index) ResourceManager::getMaterial(name_material)->get_data<type>(name_data)[index]

//#define CHECK_AVAILABLE_POS(x, y, width, height) ((x > 0 && x < width) && (y > 0 && y < height))

GameApp::GameApp()
    : Application()
    , m_nickname("defaultNick")
{
    m_ini_region_user.nickname = &m_nickname;
    m_ini_data.add_region("USER", &m_ini_region_user);
}
GameApp::~GameApp()
{}

bool GameApp::init()
{
    lock_key_update = new bool(false);

    m_cam = new Camera(glm::vec3(12.5f, 55.f, 30.f), glm::vec3(-75.f, -90.f, 0.f));

    m_line = new RenderEngine::Line(ResourceManager::getMaterial("default"), 5.f);

    m_cam->set_viewport_size(static_cast<float>(m_pWindow->get_size().x), static_cast<float>(m_pWindow->get_size().y));
    // init materials
    GET_DATA_MATERIAL("green_place", float, "sourceColor", 1) = 1.f;
    GET_DATA_MATERIAL("green_place", float, "sourceColor", 3) = 0.2f;

    GET_DATA_MATERIAL("cube", float, "ambient_factor", 0) = 0.25f;
    GET_DATA_MATERIAL("cube", float, "diffuse_factor", 0) = 0.1f;
    GET_DATA_MATERIAL("cube", float, "specular_factor", 0) = 0.0f;
    GET_DATA_MATERIAL("cube", float, "metalic_factor", 0) = 0.0f;

    GET_DATA_MATERIAL("dirt", float, "ambient_factor", 0) = 0.25f;
    GET_DATA_MATERIAL("dirt", float, "diffuse_factor", 0) = 0.1f;
    GET_DATA_MATERIAL("dirt", float, "specular_factor", 0) = 0.0f;
    GET_DATA_MATERIAL("dirt", float, "metalic_factor", 0) = 0.0f;

    GET_DATA_MATERIAL("tower", float, "ambient_factor", 0) = 0.3f;
    GET_DATA_MATERIAL("tower", float, "diffuse_factor", 0) = 0.4f;
    GET_DATA_MATERIAL("tower", float, "specular_factor", 0) = 0.0f;
    GET_DATA_MATERIAL("tower", float, "metalic_factor", 0) = 0.0f;

    GET_DATA_MATERIAL("castle", float, "ambient_factor", 0) = 0.3f;
    GET_DATA_MATERIAL("castle", float, "diffuse_factor", 0) = 0.4f;
    GET_DATA_MATERIAL("castle", float, "specular_factor", 0) = 0.0f;
    GET_DATA_MATERIAL("castle", float, "metalic_factor", 0) = 0.0f;

    GET_DATA_MATERIAL("monkey", float, "ambient_factor", 0) = 0.5f;
    GET_DATA_MATERIAL("monkey", float, "diffuse_factor", 0) = 0.4f;
    GET_DATA_MATERIAL("monkey", float, "specular_factor", 0) = 0.0f;
    GET_DATA_MATERIAL("monkey", float, "metalic_factor", 0) = 0.0f;
    GET_DATA_MATERIAL("monkey", float, "shininess", 0) = 0.1f;

    std::vector<std::string> names;
    names.push_back("default3DShader");

    m_scene.add_object<Plane>(ResourceManager::getMaterial("dirt"), glm::vec3(size_x, 0.f, size_y), glm::vec3(size_x, 0.f, size_y));
    m_scene.add_object<DirectionalLight>(names);
    m_scene.add_object<Grid>(glm::vec3(size_x, 0.5f, size_y), glm::vec2(1.f), size_x, size_y, glm::vec4(1.f, 1.f, 1.f, 0.5f), ResourceManager::getMaterial("default"));
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

    add_green_place(parts[217] - glm::vec3(1.f, 0.f, 1.f), parts[762] + glm::vec3(1.f, 0.f, 1.f));
    add_green_place(parts[0] - glm::vec3(1.f, 0.f, 1.f), parts[751] + glm::vec3(1.f, 0.f, 1.f));
    add_green_place(parts[2] - glm::vec3(1.f, 0.f, 1.f), parts[53] + glm::vec3(1.f, 0.f, 1.f));
    add_green_place(parts[78] - glm::vec3(1.f, 0.f, 1.f), parts[683] + glm::vec3(1.f, 0.f, 1.f));
    add_green_place(parts[24] - glm::vec3(1.f, 0.f, 1.f), parts[28] + glm::vec3(1.f, 0.f, 1.f));
    add_green_place(parts[29] - glm::vec3(1.f, 0.f, 1.f), parts[899] + glm::vec3(1.f, 0.f, 1.f));
    add_green_place(parts[853] - glm::vec3(1.f, 0.f, 1.f), parts[898] + glm::vec3(1.f, 0.f, 1.f));

    // game system init
    init_gui();
    start_game_single();
    //init_winSock(); // network features

    m_gui->set_active(true);

    m_gui->get_element<GUI::Button>("Disconnect")->set_active(false);

    m_gui->get_element<GUI::ChatBox>("chat_place")->set_open(false);

    m_scene.at(curObj)->getComponent<Transform>()->set_position(parts[0]);
    //  --------------------------------------------- commands init -------------------------------
    CommandManager::add_command("cen", [&](std::vector<std::string> args) {
        if (args.empty())
        {
            m_chat_mes.push("Spawn enemies count: " + std::to_string(countSpawnEnemies));
            return;
        }
        countSpawnEnemies = std::stoi(args[0]);
        m_chat_mes.push("Set spawn enemies count: " + std::to_string(countSpawnEnemies));
        //buff[1] = 'a';
        //sysfunc::type_to_char(&_set_min_distance, buff, 2);
        //WinSock::send_data(buff, sizeof(double) + 2);
        });
    CommandManager::add_command("dis", [&](std::vector<std::string> args) {
        if (args.empty())
        {
            m_chat_mes.push("Min distance: " + std::to_string(_set_min_distance));
            return;
        }
        _set_min_distance = std::stod(args[0]);
        m_chat_mes.push("Set min distance: " + std::to_string(_set_min_distance));
        //buff[1] = 'a';
        //sysfunc::type_to_char(&_set_min_distance, buff, 2);
        //WinSock::send_data(buff, sizeof(double) + 2);
        });
    CommandManager::add_command("vel", [&](std::vector<std::string> args) {
        if (args.empty())
        {
            m_chat_mes.push("Velocity: " + std::to_string(_set_velosity));
            return;
        }
        _set_velosity = std::stod(args[0]);
        m_chat_mes.push("Set velocity: " + std::to_string(_set_velosity));
        //buff[1] = 'a';
        //sysfunc::type_to_char(&_set_min_distance, buff, 2);
        //WinSock::send_data(buff, sizeof(double) + 2);
        });
    CommandManager::add_command("mhpcas", [&](std::vector<std::string> args) {
        if (args.empty())
        {
            m_chat_mes.push("Max hp castle: " + std::to_string(_set_max_hp_castle));
            return;
        }
        _set_max_hp_castle = std::stod(args[0]);
        m_chat_mes.push("Set max hp castle: " + std::to_string(_set_max_hp_castle));
        m_chat_mes.push("Need restart");
        //buff[1] = 'a';
        //sysfunc::type_to_char(&_set_min_distance, buff, 2);
        //WinSock::send_data(buff, sizeof(double) + 2);
        });
    CommandManager::add_command("mhpen", [&](std::vector<std::string> args) {
        if (args.empty())
        {
            m_chat_mes.push("Max hp enemy: " + std::to_string(_set_max_hp_enemy));
            return;
        }
        _set_max_hp_enemy = std::stod(args[0]);
        m_chat_mes.push("Set max hp enemy: " + std::to_string(_set_max_hp_enemy));
        //buff[1] = 'a';
        //sysfunc::type_to_char(&_set_min_distance, buff, 2);
        //WinSock::send_data(buff, sizeof(double) + 2);
        });
    CommandManager::add_command("cdtow", [&](std::vector<std::string> args) {
        if (args.empty())
        {
            m_chat_mes.push("Cooldown tower: " + std::to_string(_set_cooldown_tower));
            return;
        }
        _set_cooldown_tower = std::stod(args[0]);
        m_chat_mes.push("Set cooldown tower: " + std::to_string(_set_cooldown_tower));
        m_chat_mes.push("Need restart");
        //buff[1] = 'a';
        //sysfunc::type_to_char(&_set_min_distance, buff, 2);
        //WinSock::send_data(buff, sizeof(double) + 2);
        });
    CommandManager::add_command("dmgtow", [&](std::vector<std::string> args) {
        if (args.empty())
        {
            m_chat_mes.push("Damage tower: " + std::to_string(_set_damage_tower));
            return;
        }
        _set_cooldown_tower = std::stod(args[0]);
        m_chat_mes.push("Set damage tower: " + std::to_string(_set_damage_tower));
        m_chat_mes.push("Need restart");
        //buff[1] = 'a';
        //sysfunc::type_to_char(&_set_min_distance, buff, 2);
        //WinSock::send_data(buff, sizeof(double) + 2);
        });
    CommandManager::add_command("dmgen", [&](std::vector<std::string> args) {
        if (args.empty())
        {
            m_chat_mes.push("Damage enemy: " + std::to_string(_set_damage_enemy));
            return;
        }
        _set_damage_enemy = std::stod(args[0]);
        m_chat_mes.push("Set damage enemy: " + std::to_string(_set_damage_enemy));
        //buff[1] = 'a';
        //sysfunc::type_to_char(&_set_min_distance, buff, 2);
        //WinSock::send_data(buff, sizeof(double) + 2);
        });
    CommandManager::add_command("reload", [&](std::vector<std::string> args) {
        if (args.empty())
        {
            ResourceManager::load_JSON_resources("res/resources.json");
        }
        else if (args[0] == " shaders")
        {
            ResourceManager::load_JSON_shaders("res/resources.json");
        }
        else if (args[0] == " textures")
        {
            ResourceManager::load_JSON_textures("res/resources.json");
        }
        else if (args[0] == " fonts")
        {
            ResourceManager::load_JSON_fonts("res/resources.json");
        }
        });
    CommandManager::add_command("cls", [&](std::vector<std::string> args) {
        m_gui->get_element<GUI::ChatBox>("Chat")->clear();
        });
    CommandManager::add_command("help", [&](std::vector<std::string> args) {
        std::vector<std::string> commands = CommandManager::get_commands();
        std::string cmds = "";
        for (const auto cur : commands)
        {
            cmds += cur + " ";
        }
        m_chat_mes.push(cmds);
        //m_chat_mes.push("/<command> - for get value");
        //m_chat_mes.push("/<command> <arg1> ... - for use command");
        //m_chat_mes.push("/cls - clear chat");
        //m_chat_mes.push("/dis - distance attack towers");
        //m_chat_mes.push("/vel - velosity enemies");
        //m_chat_mes.push("/mhpcas - max HP castle");
        //m_chat_mes.push("/mhpen - max HP enemies");
        //m_chat_mes.push("/cdtow - cooldown tower attacks");
        //m_chat_mes.push("/dmgtow - damage tower attacks");
        //m_chat_mes.push("/reload <shaders/textures/fonts> - reload resources");
        });
    //  --------------------------------------------- commands init -------------------------------

    targets.push_back(Target(parts[814]));
    targets.push_back(Target(parts[124]));
    targets.push_back(Target(parts[135]));
    targets.push_back(Target(parts[765]));
    targets.push_back(Target(parts[776]));
    targets.push_back(Target(parts[116]));

    // --------------------------- fills dead zones -------------------------------
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 13; j++)
        {
            map[780 + 30 * i + j] = true;
        }
    }
    for (size_t i = 0; i < 24; i++)
    {
        for (size_t j = 0; j < 5; j++)
        {
            map[62 + 30 * i + j] = true;
        }
    }
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {
            map[67 + 30 * i + j] = true;
        }
    }
    for (size_t i = 0; i < 26; i++)
    {
        for (size_t j = 0; j < 5; j++)
        {
            map[73 + 30 * i + j] = true;
        }
    }
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 0; j < 6; j++)
        {
            map[708 + 30 * i + j] = true;
        }
    }
    for (size_t i = 0; i < 27; i++)
    {
        for (size_t j = 0; j < 5; j++)
        {
            map[54 + 30 * i + j] = true;
        }
    }

    return true;
}

void GameApp::terminate()
{
    delete m_cam;
    delete lock_key_update;
    WinSock::close_WinSock();
    chat_last.clear();
}

void GameApp::on_key_update(const double delta)
{// need more additions
    if (m_isLose || *lock_key_update)
    {
        return;
    }
    
    glm::vec3 movement_delta{ 0,0,0 };
    glm::vec3 rotation_delta{ 0,0,0 };

    double addSpeed = 1;

    if (Input::isKeyPressed(KeyCode::KEY_T))
    {
        if (is_chat_active && !isKeyPressed && !is_chat_full_hide && !is_gui_active)
        {
            m_gui->get_element<GUI::ChatBox>("Chat")->set_open(false);
            m_gui->get_element<GUI::GUI_element>("chat_place")->set_active(false);
            is_chat_active = false;
            isKeyPressed = true;
        }
        else if (!isKeyPressed && !is_chat_full_hide && !is_gui_active)
        {
            m_gui->get_element<GUI::GUI_element>("chat_place")->set_active(true);
            m_gui->get_element<GUI::InputField>("SendMessage")->set_focus(true);
            m_gui->get_element<GUI::ChatBox>("Chat")->set_open(true);
            m_gui->get_element<GUI::InputField>("SendMessage")->set_text("");
            is_chat_active = true;
            isKeyPressed = true;
        }
    }

    if (is_gui_active) return; // block move in menu

    if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT) && !m_gui->get_focus())
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
                    LOG_INFO("CUR: {0}", cur);
                    m_scene.at(curObj)->getComponent<Transform>()->set_position(parts[cur]);

                    isKeyPressedmouse = true;

                    if (is_spawn_mode)
                    {
                        int x = cur / size_y;
                        int y = cur % size_x;

                        isKeyPressedmouse = true;

                        char buff[sizeof(unsigned int) + 1];

                        if (m_mode == GameMode::Single)
                        {
                            if (is_spawn_enemy)
                            {
                                /*for (unsigned int i = 0; i < countSpawnEnemies; i++)
                                {
                                    m_spawn_enemies.push(cur);
                                }
                                buff[0] = 's';
                                sysfunc::type_to_char(&cur, buff, 1);

                                WinSock::send_data(buff, sizeof(unsigned int) + 1);*/
                            }
                            else if (!map[cur])
                            {
                                map[cur] = true;
                                m_spawn_towers.push(cur);
                                buff[0] = 't';
                                sysfunc::type_to_char(&cur, buff, 1);

                                WinSock::send_data(buff, sizeof(unsigned int) + 1);
                                //LOG_INFO("Add tower at {0}x{1}", x, y);
                            }
                        }
                        else
                        {
                            if (is_spawn_enemy)
                            {
                               /* if (isServer)
                                {
                                    if (x < 5 && y > 16 && y < 30)
                                    {
                                        for (unsigned int i = 0; i < countSpawnEnemies; i++)
                                        {
                                            m_spawn_enemies_self.push(cur);
                                        }
                                        buff[0] = 's';
                                        sysfunc::type_to_char(&cur, buff, 1);

                                        WinSock::send_data(buff, sizeof(unsigned int) + 1);
                                    }
                                }
                                else
                                {
                                    if (x > 25 && y < 14 && y > 1)
                                    {
                                        for (unsigned int i = 0; i < countSpawnEnemies; i++)
                                        {
                                            m_spawn_enemies_self.push(cur);
                                        }
                                        buff[0] = 's';
                                        sysfunc::type_to_char(&cur, buff, 1);

                                        WinSock::send_data(buff, sizeof(unsigned int) + 1);
                                    }
                                }*/
                            }
                            else if (!map[cur])
                            {
                                if (isServer)
                                {
                                    if ((x > 6 && x < 25) && ((y > 0 && y < 3) || (y > 8 && y < 16)))
                                    {
                                        map[cur] = true;
                                        m_spawn_towers.push(cur);
                                        buff[0] = 't';
                                        sysfunc::type_to_char(&cur, buff, 1);

                                        WinSock::send_data(buff, sizeof(unsigned int) + 1);
                                    }
                                }
                                else
                                {
                                    if ((x > 6 && x < 25) && ((y > 16 && y < 19) || (y > 22 && y < 30)))
                                    {
                                        map[cur] = true;
                                        m_spawn_towers.push(cur);
                                        buff[0] = 't';
                                        sysfunc::type_to_char(&cur, buff, 1);

                                        WinSock::send_data(buff, sizeof(unsigned int) + 1);
                                    }
                                }
                            }
                        }

                        //LOG_INFO("Pos: X:{0} Y:{1}", x, y);
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

                    isKeyPressedmouse = true;

                    char buff[sizeof(unsigned int) + 1];

                    if (m_mode == GameMode::Single)
                    {
                        if (is_spawn_enemy)
                        {
                            /*for (unsigned int i = 0; i < countSpawnEnemies; i++)
                            {
                                m_spawn_enemies.push(cur);
                            }
                            buff[0] = 's';
                            sysfunc::type_to_char(&cur, buff, 1);

                            WinSock::send_data(buff, sizeof(unsigned int) + 1);*/
                        }
                        else if (!map[cur])
                        {
                            map[cur] = true;
                            m_spawn_towers.push(cur);
                            buff[0] = 't';
                            sysfunc::type_to_char(&cur, buff, 1);

                            WinSock::send_data(buff, sizeof(unsigned int) + 1);
                            //LOG_INFO("Add tower at {0}x{1}", x, y);
                        }
                    }
                    else
                    {
                        if (is_spawn_enemy)
                        {
                            /*if (isServer)
                            {
                                if (x < 5 && y > 16 && y < 30)
                                {
                                    m_spawn_enemies_self.push(cur);
                                    buff[0] = 's';
                                    sysfunc::type_to_char(&cur, buff, 1);

                                    WinSock::send_data(buff, sizeof(unsigned int) + 1);
                                }
                            }
                            else
                            {
                                if (x > 25 && y < 14 && y > 1)
                                {
                                    m_spawn_enemies_self.push(cur);
                                    buff[0] = 's';
                                    sysfunc::type_to_char(&cur, buff, 1);

                                    WinSock::send_data(buff, sizeof(unsigned int) + 1);
                                }
                            }*/
                        }
                        else if (!map[cur])
                        {
                            if (isServer)
                            {
                                if ((x > 6 && x < 25) && ((y > 0 && y < 3) || (y > 8 && y < 16)))
                                {
                                    map[cur] = true;
                                    m_spawn_towers.push(cur);
                                    buff[0] = 't';
                                    sysfunc::type_to_char(&cur, buff, 1);

                                    WinSock::send_data(buff, sizeof(unsigned int) + 1);
                                }
                            }
                            else
                            {
                                if ((x > 6 && x < 25) && ((y > 16 && y < 19) || (y > 22 && y < 30)))
                                {
                                    map[cur] = true;
                                    m_spawn_towers.push(cur);
                                    buff[0] = 't';
                                    sysfunc::type_to_char(&cur, buff, 1);

                                    WinSock::send_data(buff, sizeof(unsigned int) + 1);
                                }
                            }
                        }
                    }

                    //LOG_INFO("Pos: X:{0} Y:{1}", x, y);
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
    if (is_lock_move) return;
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

    if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_MIDDLE) || moveCamMoue)
    {
        glm::vec2 pos = m_pWindow->get_current_cursor_position();
        rotation_delta.y = m_isInversiveMouseY
            ? (rotation_delta.y + ((m_init_mouse_pos_x - pos.x) / (1 / m_cam_sensetivity)))
            : (rotation_delta.y - ((m_init_mouse_pos_x - pos.x) / (1 / m_cam_sensetivity)));
        rotation_delta.x += (m_init_mouse_pos_y - pos.y) / (1 / m_cam_sensetivity);
        m_init_mouse_pos_x = pos.x;
        m_init_mouse_pos_y = pos.y;
    }

    if (is_special_move)
    {
        movDel += movement_delta;
        rotDel += rotation_delta;
        return;
    }
    m_cam->add_movement_and_rotation(movement_delta, rotation_delta);
}

void GameApp::on_update(const double delta)
{       
    if (is_special_move)
    {
        m_cam->add_movement_and_rotation(movDel, rotDel);
        movDel = glm::vec3(0);
        rotDel = glm::vec3(0);
    }

    m_gui->get_element<GUI::TextRenderer>("cam_pos")->set_text("Cam pos: " +
        std::to_string(m_cam->get_position().x) + " " + std::to_string(m_cam->get_position().y) + " " + std::to_string(m_cam->get_position().z));

    m_gui->on_update(delta);

    for (size_t i = 0; i < m_scene.get_list().size(); i++)
    {   
        m_scene.at(i)->update(delta);
    }

    // ================================================================================ game logic ===========================================================
    if (restart_querry)
    {
        if (m_mode == GameMode::Single)
        {
            start_game_single();
        }
        else
        {
            start_game_multi();
        }
        restart_querry = false;
    }

    if (m_mode == GameMode::Single)
    {
        if (m_isLose) return;

        m_main_castle->update(delta);

        if (m_main_castle->isDestroyed() && !m_isLose)
        {
            m_isLose = true;
            m_gui->get_element<GUI::GUI_element>("menu_place")->set_active(false);
            m_gui->get_element<GUI::GUI_element>("settings_place")->set_active(false);
            m_gui->get_element<GUI::GUI_element>("multiplayer_menu_place")->set_active(false);
            m_gui->get_element<GUI::TextRenderer>("Lose scoreboard")->set_text("Your score: " + std::to_string(countKills));
            m_gui->get_element<GUI::GUI_element>("lose_place")->set_active(true);
            //if (isServer) WinSock::send_data("l", 1);
        }
        // sync

        while (!m_spawn_towers.empty())
        {
            m_towers.push_back({ new BaseTower(ResourceManager::get_OBJ_model("tower"),
                ResourceManager::getMaterial("tower"), nullptr, parts[m_spawn_towers.front()],
                _set_cooldown_tower, _set_damage_tower, new RenderEngine::Line(ResourceManager::getMaterial("default"))), m_spawn_towers.front()});
            ResourceManager::get_unique_sound("tower_spawn")->play();
            m_spawn_towers.pop();
        }
        while (!m_spawn_enemies.empty()) // spawn enemies
        {
            glm::vec3 pos = parts[m_spawn_enemies.front()];
            countEnemiesPerm++;
            auto a = new BaseEnemy(new ObjModel(ResourceManager::get_OBJ_model("monkey"), ResourceManager::getMaterial("monkey")),
                m_main_castle, targets, glm::vec3(pos.x + (rand() % 100 - 50) / 100.f, pos.y, pos.z + (rand() % 100 - 50) / 100.f), 1, _set_velosity * 0.001,
                _set_max_hp_enemy, _set_damage_enemy, ResourceManager::getMaterial("default"));
            m_enemies.push_back(a);
            ResourceManager::get_sound("enemy_spawn")->play();
            m_spawn_enemies.pop();
            m_gui->get_element<GUI::TextRenderer>("enemies")->set_text("Enemies: " + std::to_string(countEnemiesPerm));
        }

        for (auto curTower : m_towers)
        {
            double distance = 100; // <-------
            for (size_t i = 0; i < m_enemies.size(); i++)
            {
                if (m_enemies[i] == nullptr) continue;
                glm::vec3 a = m_enemies[i]->get_pos() - curTower.tow->get_pos();
                double d = sqrt(a.x * a.x + a.z * a.z);
                if (d < _set_min_distance && d < distance)
                {
                    distance = d;
                    if (curTower.tow->get_target() == nullptr) curTower.tow->set_target(m_enemies[i]);
                    //if (curTower->get_target() != m_enemies[i]) curTower->set_target(m_enemies[i]);
                }
                else
                {
                    //curTower->set_target(nullptr);
                }
            }
            curTower.tow->update(delta);
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
                    if (curTower.tow->get_target() == m_enemies[i]) curTower.tow->set_target(nullptr);
                }
                delete m_enemies[i];
                m_enemies.remove(i);
                countKills++;
                m_gui->get_element<GUI::TextRenderer>("kills")->set_text("Kills: " + std::to_string(countKills));
                m_gui->get_element<GUI::TextRenderer>("enemies")->set_text("Enemies: " + std::to_string(countEnemiesPerm));
                continue;
            }
            m_enemies[i]->update(delta);
        }
    }    
    //else
    //{
    //    m_main_castle->update(delta);
    //    m_adv_castle->update(delta);
    //    while (!m_spawn_towers.empty())
    //    {
    //        m_towers.push_back(new BaseTower(ResourceManager::load_OBJ_file("res/models/tower.obj"),
    //            ResourceManager::getMaterial("tower"), nullptr, parts[m_spawn_towers.front()],
    //            _set_cooldown_tower, _set_damage_tower, new RenderEngine::Line(ResourceManager::getMaterial("default"))));
    //        m_spawn_towers.pop();
    //    }
    //    while (!m_spawn_towers_self.empty())
    //    {
    //        m_towers_self.push_back(new BaseTower(ResourceManager::load_OBJ_file("res/models/tower.obj"),
    //            ResourceManager::getMaterial("tower"), nullptr, parts[m_spawn_towers_self.front()],
    //            _set_cooldown_tower, _set_damage_tower, new RenderEngine::Line(ResourceManager::getMaterial("default"))));
    //        m_spawn_towers_self.pop();
    //    }
    //    while (!m_spawn_enemies.empty()) // spawn enemies adv
    //    {
    //        countEnemiesPerm++;
    //        auto a = new BaseEnemy(new ObjModel("res/models/monkey.obj", ResourceManager::getMaterial("monkey")),
    //            isServer ? m_adv_castle : m_main_castle, parts[m_spawn_enemies.front()], 1, _set_velosity * 0.001, 
    //            _set_max_hp_enemy, ResourceManager::getMaterial("default"), isServer ? glm::vec3(0.f, 0.f, 1.f) : glm::vec3(1.f, 0.f, 0.f));
    //        m_enemies.push_back(a);
    //        m_spawn_enemies.pop();
    //        m_gui->get_element<GUI::TextRenderer>("enemies")->set_text("Enemies: " + std::to_string(countEnemiesPerm));
    //    }
    //    while (!m_spawn_enemies_self.empty()) // spawn enemies adv
    //    {
    //        countEnemiesPerm++;
    //        auto a = new BaseEnemy(new ObjModel("res/models/monkey.obj", ResourceManager::getMaterial("monkey")),
    //            isServer ? m_main_castle : m_adv_castle, parts[m_spawn_enemies_self.front()], 1, _set_velosity * 0.001,
    //            _set_max_hp_enemy, ResourceManager::getMaterial("default"), isServer ? glm::vec3(1.f, 0.f, 0.f) : glm::vec3(0.f, 0.f, 1.f));
    //        m_enemies_self.push_back(a);
    //        m_spawn_enemies_self.pop();
    //        m_gui->get_element<GUI::TextRenderer>("enemies")->set_text("Enemies: " + std::to_string(countEnemiesPerm));
    //    }
    //    for (auto curTower : m_towers)
    //    {
    //        double distance = 100; // <-------
    //        for (size_t i = 0; i < m_enemies_self.size(); i++)
    //        {
    //            if (m_enemies_self[i] == nullptr) continue;
    //            glm::vec3 a = m_enemies_self[i]->get_pos() - curTower->get_pos();
    //            double d = sqrt(a.x * a.x + a.z * a.z);
    //            if (d < _set_min_distance && d < distance)
    //            {
    //                distance = d;
    //                if (curTower->get_target() == nullptr) curTower->set_target(m_enemies_self[i]);
    //                //if (curTower->get_target() != m_enemies[i]) curTower->set_target(m_enemies[i]);
    //            }
    //            else
    //            {
    //                //curTower->set_target(nullptr);
    //            }
    //        }
    //        curTower->update(delta);
    //    }
    //    for (auto curTower1 : m_towers_self)
    //    {
    //        double distance = 100; // <-------
    //        for (size_t i = 0; i < m_enemies.size(); i++)
    //        {
    //            if (m_enemies[i] == nullptr) continue;
    //            glm::vec3 a = m_enemies[i]->get_pos() - curTower1->get_pos();
    //            double d = sqrt(a.x * a.x + a.z * a.z);
    //            if (d < _set_min_distance && d < distance)
    //            {
    //                distance = d;
    //                if (curTower1->get_target() == nullptr) curTower1->set_target(m_enemies[i]);
    //                //if (curTower->get_target() != m_enemies[i]) curTower->set_target(m_enemies[i]);
    //            }
    //            else
    //            {
    //                //curTower->set_target(nullptr);
    //            }
    //        }
    //        curTower1->update(delta);
    //    }
    //    for (size_t i = 0; i < m_enemies.size(); i++)
    //    {
    //        if (m_enemies[i] == nullptr) continue;
    //        if (m_enemies[i]->is_destroy())
    //        {
    //            //countEnemies++;
    //            countEnemiesPerm--;
    //            for (auto curTower : m_towers)
    //            {
    //                if (curTower->get_target() == m_enemies[i]) curTower->set_target(nullptr);
    //            }
    //            delete m_enemies[i];
    //            m_enemies.remove(i);
    //            countKills++;
    //            m_gui->get_element<GUI::TextRenderer>("kills")->set_text("Kills: " + std::to_string(countKills));
    //            m_gui->get_element<GUI::TextRenderer>("enemies")->set_text("Enemies: " + std::to_string(countEnemiesPerm));
    //            continue;
    //        }
    //        m_enemies[i]->update(delta);
    //    }
    //    for (size_t i = 0; i < m_enemies_self.size(); i++)
    //    {
    //        if (m_enemies_self[i] == nullptr) continue;
    //        if (m_enemies_self[i]->is_destroy())
    //        {
    //            //countEnemies++;
    //            countEnemiesPerm--;
    //            for (auto curTower : m_towers)
    //            {
    //                if (curTower->get_target() == m_enemies_self[i]) curTower->set_target(nullptr);
    //            }
    //            delete m_enemies_self[i];
    //            m_enemies_self.remove(i);
    //            countKills++;
    //            m_gui->get_element<GUI::TextRenderer>("kills")->set_text("Kills: " + std::to_string(countKills));
    //            m_gui->get_element<GUI::TextRenderer>("enemies")->set_text("Enemies: " + std::to_string(countEnemiesPerm));
    //            continue;
    //        }
    //        m_enemies_self[i]->update(delta);
    //    }
    //}

    // ================================================================================ game logic ===========================================================
    // tps counter
    if (ticks < 5)
    {
        ticks++;
        timesTicks += delta;
    }
    else
    {
        tps = int((ticks / timesTicks) * 1000.f);
        m_gui->get_element<GUI::TextRenderer>("tps")->set_text("TPS: " + std::to_string(tps));
        ticks = 0;
        timesTicks = 0;
    }
}

void GameApp::on_render(const double delta)
{// clear screen
    RenderEngine::Renderer::setClearColor(m_colors[0], m_colors[1], m_colors[2], m_colors[3]);

    RenderEngine::Renderer::clearColor();
    // set matrix
    ResourceManager::getShaderProgram("colorShader")->use();
    ResourceManager::getShaderProgram("colorShader")->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_cam->get_projection_matrix() * m_cam->get_view_matrix());
    //ResourceManager::getShaderProgram("colorShader")->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_cam->get_ui_matrix());

    ResourceManager::getShaderProgram("default3DShader")->use();
    ResourceManager::getShaderProgram("default3DShader")->setVec3("cam_position", m_cam->get_position());

    m_scene.at(4)->getComponent<Transform>()->set_position(parts[cur_player]);
    if (is_line_active)
    {
        for (int i = 0; i < targets.size() - 1; i++)
        {
            m_line->render_from_to(targets[i].get_pos(), targets[i + 1].get_pos(), glm::vec3(1.f, 0.3f, 0.1f));
        }
    }

    m_main_castle->render();

    for (size_t i = 0; i < m_scene.get_list().size(); i++)
    {
        MeshRenderer* mesh = m_scene.at(i)->getComponent<MeshRenderer>();
        if (mesh != nullptr)
        {            
            std::shared_ptr<RenderEngine::ShaderProgram> shader = mesh->get_material_ptr()->get_shader_ptr();
            shader->use();            
            shader->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_cam->get_projection_matrix() * m_cam->get_view_matrix());
        }
        if (i != 2) m_scene.at(i)->render();
        else if (i == 2 && is_grid_active) m_scene.at(i)->render();
        if (!is_green_place_active && i == 4) break;
    }

    for (auto curTower : m_towers)
    {        
        curTower.tow->render();
    }
    for (size_t i = 0; i < m_enemies.size(); i++)
    {
        if (m_enemies[i] == nullptr) continue;
        m_enemies[i]->render();
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
        m_gui->get_element<GUI::TextRenderer>("fps")->set_text("FPS: " + std::to_string(fps));
        frames = 0;
        times = 0;
    }
}

void GameApp::on_ui_render()
{
    // for correct write message in other thread
    while (!m_chat_mes.empty())
    {
        m_gui->get_element<GUI::ChatBox>("Chat")->add_message(m_chat_mes.front());
        LOG_INFO("[CHAT] {0}", m_chat_mes.front());
        m_chat_mes.pop();
    }
    // =========================================
    m_gui->on_render();
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
            press_button(e.key_code);
            m_gui->get_element<GUI::InputField>("InputNick")->press_button(e.key_code);
            m_gui->get_element<GUI::InputField>("InputIP")->press_button(e.key_code);
            m_gui->get_element<GUI::InputField>("SendMessage")->press_button(e.key_code);
        });
    m_event_dispather.add_event_listener<EventCharSet>([&](EventCharSet& e)
        {
            m_gui->get_element<GUI::InputField>("InputNick")->press_char(e.key_char);
            m_gui->get_element<GUI::InputField>("InputIP")->press_char(e.key_char);
            m_gui->get_element<GUI::InputField>("SendMessage")->press_char(e.key_char);
        });
    m_event_dispather.add_event_listener<EventKeyReleased>([&](EventKeyReleased& e)
        {
            if (e.key_code <= KeyCode::KEY_Z)  if (is_event_logging_active) LOG_INFO("[EVENT] Key released {0}", static_cast<char>(e.key_code));
            isKeyPressed = false;

            Input::releaseKey(e.key_code);
        });
    m_event_dispather.add_event_listener<EventMouseMoved>([&](EventMouseMoved& e)
        {
            m_gui->get_element<GUI::Slider>("slider_volume")->move_mouse(e.x);
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
            if (is_chat_active) m_gui->get_element<GUI::ChatBox>("Chat")->on_scroll(-e.y_offset);
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
            if (is_event_logging_active) LOG_INFO("[EVENT] Mouse button pressed at ({0}x{1})", e.x_pos, e.y_pos);

            Input::pressMouseButton(e.mouse_button);
            m_init_mouse_pos_x = e.x_pos;
            m_init_mouse_pos_y = e.y_pos;
        });
    m_event_dispather.add_event_listener<EventMouseButtonReleased>([&](EventMouseButtonReleased& e)
        {
            isKeyPressedmouse = false;
            m_gui->on_mouse_release(e.x_pos, e.y_pos);
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

void GameApp::press_button(KeyCode key)
{    
    if (m_isLose || *lock_key_update)
    {
        if (key == KeyCode::KEY_ESCAPE)
        {
            m_gui->get_element<GUI::InputField>("SendMessage")->set_focus(false);
            m_gui->get_element<GUI::InputField>("SendMessage")->set_active(false);
            isKeyPressed = true;
            if (is_chat_active)
            {
                is_chat_active = false;
                m_gui->get_element<GUI::ChatBox>("Chat")->set_open(false);
            }
        }
        if (m_gui->get_element<GUI::InputField>("SendMessage")->get_focus())
        {
            if (key == KeyCode::KEY_UP && !isKeyPressed)
            {
                if (cur_chat_index < chat_last.size() - 1) cur_chat_index++;
                if (!chat_last.empty()) m_gui->get_element<GUI::InputField>("SendMessage")->set_text(chat_last[cur_chat_index]);
                isKeyPressed = true;
            }
            else if (key == KeyCode::KEY_DOWN && !isKeyPressed)
            {
                if (cur_chat_index > 0) cur_chat_index--;
                if (!chat_last.empty()) m_gui->get_element<GUI::InputField>("SendMessage")->set_text(chat_last[cur_chat_index]);
                isKeyPressed = true;
            }
        }
        return;
    }

    if (key == k_debug)
    {
        if (!isKeyPressed)
        {
            m_gui->get_element<GUI::GUI_element>("debug_place")->set_active(!m_gui->get_element<GUI::GUI_element>("debug_place")->get_active());
            isKeyPressed = true;
        }
    }
    switch (key)
    {
    case KeyCode::KEY_F1:
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
        break;
    case KeyCode::KEY_ESCAPE:
        if (is_gui_active && !isKeyPressed)
        {
            m_gui->get_element<GUI::GUI_element>("menu_place")->set_active(false);
            m_gui->get_element<GUI::GUI_element>("settings_place")->set_active(false);
            m_gui->get_element<GUI::GUI_element>("multiplayer_menu_place")->set_active(false);
            is_gui_active = false;
            isKeyPressed = true;
        }
        else if (!isKeyPressed)
        {
            m_gui->get_element<GUI::GUI_element>("multiplayer_menu_place")->set_active(false);
            m_gui->get_element<GUI::GUI_element>("settings_place")->set_active(false);
            is_gui_active = true;
            m_gui->get_element<GUI::GUI_element>("menu_place")->set_active(true);
            isKeyPressed = true;
        }
        break;
    case KeyCode::KEY_J:
        if (!isKeyPressed)
        {
            int add = rand() % 10;
            unsigned int spawn = 841 + add;
            for (unsigned int i = 0; i < countSpawnEnemies; i++)
            {
                m_spawn_enemies.push(spawn);
            }
            isKeyPressed = true;
        }
        break;
    case KeyCode::KEY_L:
        if (is_green_place_active && !isKeyPressed)
        {
            is_green_place_active = false;
            //m_gui->set_logging_active(false);
            isKeyPressed = true;
        }
        else if (!isKeyPressed)
        {
            is_green_place_active = true;
            //m_gui->set_logging_active(true);
            isKeyPressed = true;
        }
        break;
    case KeyCode::KEY_M:
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
        break;
    case KeyCode::KEY_P:
        if (!isKeyPressed)
        {
            glm::vec3 pos = m_cam->get_position();
            glm::vec3 rot = m_cam->get_rotation();
            LOG_INFO("Cam pos: {0} {1} {2}", pos.x, pos.y, pos.z);
            LOG_INFO("Cam rot: {0} {1} {2}", rot.x, rot.y, rot.z);
            m_cam->set_position_rotation(glm::vec3(12.5f, 55.f, 30.f), glm::vec3(-75.f, -90.f, 0.f));
            isKeyPressed = true;
        }
        break;
    case KeyCode::KEY_R:
        if (is_line_active && !isKeyPressed)
        {
            is_line_active = false;
            isKeyPressed = true;
        }
        else if (!isKeyPressed)
        {
            is_line_active = true;
            isKeyPressed = true;
        }
        break;
    case KeyCode::KEY_G:
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
        break;
    }    
}

void GameApp::init_gui()
{
    ResourceManager::get_font("calibri")->set_scale(0.5f);
    ResourceManager::get_font("calibriChat")->set_scale(0.27f);

    m_gui = new GUI::GUI_place(m_cam, ResourceManager::getMaterial("default"));

    // ------------------------------------------------------ debug panel ------------------------------------------------------------------------
    float offset = 98.f;

    glm::vec3 colorText(1.f);

    auto debug = m_gui->add_element<GUI::GUI_element>(1, "debug_place");

    m_gui->add_element<GUI::TextRenderer>(debug, ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"),
        BUILD_NAME, colorText, glm::vec2(0.1f, offset), glm::vec2(0.5f), "version_build", false);
    offset -= 2.5f;

    m_gui->add_element<GUI::TextRenderer>(debug, ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"),
        "FPS: 0", colorText, glm::vec2(0.1f, offset), glm::vec2(0.5f), "fps", false);
    offset -= 2.5f;

    m_gui->add_element<GUI::TextRenderer>(debug, ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"),
        "TPS: 0", colorText, glm::vec2(0.1f, offset), glm::vec2(0.5f), "tps", false);
    offset -= 2.5f;

    m_gui->add_element<GUI::TextRenderer>(debug, ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"),
        "Enemies: 0", colorText, glm::vec2(0.1f, offset), glm::vec2(0.5f), "enemies", false);
    offset -= 2.5f;

    m_gui->add_element<GUI::TextRenderer>(debug, ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"),
        "Kills: 0", colorText, glm::vec2(0.1f, offset), glm::vec2(0.5f), "kills", false);
    offset -= 2.5f;

    m_gui->add_element<GUI::TextRenderer>(debug, ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"),
        "Ping: 0", colorText, glm::vec2(0.1f, offset), glm::vec2(0.5f), "ping", false);
    offset -= 2.5f;

    m_gui->add_element<GUI::TextRenderer>(debug, ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"),
        "Cam pos: 0", colorText, glm::vec2(0.1f, offset), glm::vec2(0.5f), "cam_pos", false);

    debug->set_active(false);

    // ------------------------------------------------------ chat ------------------------------------------------------------------------

    auto chat = m_gui->add_element<GUI::GUI_element>("chat_place");

    m_gui->add_element<GUI::ChatBox>(chat, new GUI::Sprite(ResourceManager::getMaterial("defaultSprite")),
        glm::vec2(13.f, 36.f), glm::vec2(12.f, 30.f), "Chat", 128, ResourceManager::get_font("calibriChat"), ResourceManager::getShaderProgram("textShader"), glm::vec3(1.f));

    m_gui->add_element<GUI::InputField>(chat, new GUI::Sprite(ResourceManager::getMaterial("default_input"), "static"),
        glm::vec2(13.f, 3.f), glm::vec2(12.f, 3.f), "SendMessage", ResourceManager::getShaderProgram("textShader"),
        ResourceManager::get_font("calibriChat"), glm::vec3(1.f), true, lock_key_update)->set_enter_callback([&](std::string text) {
            if (text[0] == '/') // need finalize
            {
                //char buff[sizeof(double) + 2];
                //buff[0] = 'c';
                
                std::vector<std::string> args;

                std::string name = text.substr(1, text.find(' '));                            
                std::string argsStr = text.substr(name.length() + 1);
                if (name.find(' ') != -1) name.pop_back();

                while (argsStr.find(' ') != -1 && sysfunc::is_full(argsStr.c_str(), argsStr.size(), ' '))
                {
                    args.push_back(argsStr.substr(0, argsStr.find(' ')));
                    argsStr = argsStr.substr(argsStr.find(' ') + 1);
                }
                if (!argsStr.empty()) args.push_back(argsStr);
                if (!CommandManager::call_command(name, args))  m_chat_mes.push("Invalid command: " + name);
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

    // ------------------------------------------------------------ lose window ------------------------------------------------

    auto lose = m_gui->add_element<GUI::GUI_element>("lose_place");

    m_gui->add_element<GUI::TextRenderer>(lose, ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "You lose!", glm::vec3(1.f, 0.1f, 0.1f), glm::vec2(41.f, 57.f), glm::vec2(2.f), "Lose text");

    m_gui->add_element<GUI::TextRenderer>(lose, ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Your score: 0", glm::vec3(1.f, 0.1f, 0.1f), glm::vec2(41.f, 50.f), glm::vec2(2.f), "Lose scoreboard");

    m_gui->add_element<GUI::Button>(lose, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 6.f), glm::vec2(10.f, 5.f),
        "Restart", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f));

    m_gui->get_element<GUI::GUI_element>("Restart")->set_click_callback([&]()
        {
            if (isServer) WinSock::send_data("r", 1);
            restart_querry = true;
        });

    // ------------------------------------------------------------ settings ------------------------------------------------
    auto settings = m_gui->add_element<GUI::GUI_element>("settings_place");

    m_gui->add_element<GUI::TextRenderer>(settings, ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Volume", glm::vec3(1.f), glm::vec2(89.f, 92.f), glm::vec2(1.f));

    m_gui->add_element<GUI::Slider>(settings, new GUI::Sprite(ResourceManager::getMaterial("slider_bg"), "static"),
        new GUI::Sprite(ResourceManager::getMaterial("slider_face"), "static"), glm::vec2(89.f, 83.f), glm::vec2(10.f, 5.f), 0, 10, "slider_volume")->
        set_slide_callback([&](float val)
            {
                volume = val / 10.f;
                SoundEngine::set_volume(volume);
            });

    m_gui->get_element<GUI::Slider>("slider_volume")->set_value(5);

    volume = 5.f / 10.f;

    SoundEngine::set_volume(volume);

    m_gui->add_element<GUI::Sprite>(settings, 1, ResourceManager::getMaterial("defaultSprite"), "default",
        glm::vec2(100.f), glm::vec2(100.f), "BG");

    m_gui->add_element<GUI::TextRenderer>(settings, ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Settings", glm::vec3(1.f), glm::vec2(50.f, 90.f), glm::vec2(1.f));

    // right    

    m_gui->add_element<GUI::BindButton>(settings, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(11.f, 50.f), glm::vec2(5.f, 5.f), ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), 
        glm::vec3(1.f), &k_debug, "BindDebug");


    // left

    m_gui->add_element<GUI::Button>(settings, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(67.f, 61.f), glm::vec2(10.f, 5.f),
        "Off spec move", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f), "SpecMoveToggle")->set_click_callback([&]()
            {
                is_special_move = !is_special_move;
                m_gui->get_element<GUI::Button>("SpecMoveToggle")->set_text(is_special_move ? "Off spec move" : "On spec move");
            });

    m_gui->add_element<GUI::Button>(settings, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 72.f), glm::vec2(10.f, 5.f),
        "Mute", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f), "muteToggle")->set_click_callback([&]()
            {
                is_mute = !is_mute;
                m_gui->get_element<GUI::Button>("muteToggle")->set_text(is_mute ? "Unmute" : "Mute");
                SoundEngine::set_volume(is_mute ? 0 : volume);
                m_gui->get_element<GUI::Slider>("slider_volume")->set_value(is_mute ? 0 : volume * 10);
            });

    m_gui->add_element<GUI::Button>(settings, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 61.f), glm::vec2(10.f, 5.f),
        "Unlock move", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f), "moveToggle")->set_click_callback([&]()
            {
                is_lock_move = !is_lock_move;
                m_gui->get_element<GUI::Button>("moveToggle")->set_text(is_lock_move ? "Unlock move" : "Lock move");
            });

    m_gui->add_element<GUI::Button>(settings, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 50.f), glm::vec2(10.f, 5.f),
        "Switch mode", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
               /* if (m_mode == GameMode::Single)
                {
                    m_mode = GameMode::Multi;
                }
                else 
                {
                    m_mode = GameMode::Single;
                }
                restart_querry = true;*/
                m_chat_mes.push("Coming soon...");
            });

    m_gui->add_element<GUI::Button>(settings, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 39.f), glm::vec2(10.f, 5.f),
        "Clear chat", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                m_gui->get_element<GUI::ChatBox>("Chat")->clear();
            });   

    m_gui->add_element<GUI::Button>(settings, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 28.f), glm::vec2(10.f, 5.f),
        "Add enemy", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                int add = rand() % 10;
                unsigned int spawn = 841 + add;
                for (unsigned int i = 0; i < countSpawnEnemies; i++)
                {
                    m_spawn_enemies.push(spawn);
                }

                //char buff[sizeof(unsigned int) + 1];
                //buff[0] = 's';
                //sysfunc::type_to_char(&spawn, buff, 1);
                //WinSock::send_data(buff, sizeof(unsigned int) + 1);          
            });;

    m_gui->add_element<GUI::Button>(settings, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(67.f, 28.f), glm::vec2(10.f, 5.f),
        "Light", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                is_green_place_active = !is_green_place_active;
            });


    m_gui->add_element<GUI::Button>(settings, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(67.f, 17.f), glm::vec2(10.f, 5.f),
        "Line", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                is_line_active = !is_line_active;
            });

    m_gui->add_element<GUI::Button>(settings, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 17.f), glm::vec2(10.f, 5.f),
        "Grid", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                is_grid_active = !is_grid_active;
            });

    m_gui->add_element<GUI::Button>(settings, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 6.f), glm::vec2(10.f, 5.f),
        "Back", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f), "BFromSettings")->set_click_callback([&]()
            {
                m_gui->get_element<GUI::GUI_element>("settings_place")->set_active(false);
                m_gui->get_element<GUI::GUI_element>("menu_place")->set_active(true);
            });

    
    // ------------------------------------------------------------ main menu ------------------------------------------------
    auto menu = m_gui->add_element<GUI::GUI_element>("menu_place");

    m_gui->add_element<GUI::TextRenderer>(menu, ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Main menu", glm::vec3(1.f), glm::vec2(50.f, 90.f), glm::vec2(1.f));

    m_gui->add_element<GUI::Button>(menu, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 6.f), glm::vec2(10.f, 5.f),
        "Quit", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                m_pCloseWindow = true;
            });

    m_gui->add_element<GUI::Button>(menu, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 39.f), glm::vec2(10.f, 5.f),
        "Multiplayer", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                m_gui->get_element<GUI::GUI_element>("menu_place")->set_active(false);
                m_gui->get_element<GUI::GUI_element>("multiplayer_menu_place")->set_active(true);
            });

    m_gui->add_element<GUI::Button>(menu, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 28.f), glm::vec2(10.f, 5.f),
        "Settings", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f))->set_click_callback([&]()
            {
                m_gui->get_element<GUI::GUI_element>("menu_place")->set_active(false);
                m_gui->get_element<GUI::GUI_element>("settings_place")->set_active(true);
            });
    
    m_gui->add_element<GUI::Button>(menu, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
            glm::vec2(89.f, 17.f), glm::vec2(10.f, 5.f),
            "Restart", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f), "RestartInMenu")->set_click_callback([&]()
                {
                    if (isServer) WinSock::send_data("r", 1);
                    restart_querry = true;
                    is_gui_active = false;
                    m_gui->get_element<GUI::GUI_element>("menu_place")->set_active(false);
                });     

    m_gui->add_element<GUI::Sprite>(menu, 1, ResourceManager::getMaterial("defaultSprite"), "default",
        glm::vec2(100.f), glm::vec2(100.f), "BG");

    // ------------------------------------------------------------ multiplayer menu ------------------------------------------------

    auto multmenu = m_gui->add_element<GUI::GUI_element>("multiplayer_menu_place");

    m_gui->add_element<GUI::TextRenderer>(multmenu, ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Multiplayer menu", glm::vec3(1.f), glm::vec2(50.f, 90.f), glm::vec2(1.f));

    m_gui->add_element<GUI::Sprite>(multmenu, 1, ResourceManager::getMaterial("defaultSprite"), "default",
        glm::vec2(100.f), glm::vec2(100.f), "BG");

    m_gui->add_element<GUI::Button>(multmenu, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(89.f, 6.f), glm::vec2(10.f, 5.f),
        "Back", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f), "BFromMultMenu")->set_click_callback([&]()
            {
                m_gui->get_element<GUI::GUI_element>("multiplayer_menu_place")->set_active(false);
                m_gui->get_element<GUI::GUI_element>("menu_place")->set_active(true);
            });

    m_gui->add_element<GUI::TextRenderer>(multmenu, ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Enter Nickname", glm::vec3(1.f), glm::vec2(88.f, 88.f), glm::vec2(1.f), "m1");

    m_gui->add_element<GUI::InputField>(multmenu, new GUI::Sprite(ResourceManager::getMaterial("default_input"), "static"),
        glm::vec2(88.f, 80.f), glm::vec2(10.f, 5.f), "InputNick", ResourceManager::getShaderProgram("textShader"),
        ResourceManager::get_font("calibri"), glm::vec3(1.f), false, lock_key_update)->set_enter_callback([&](std::string nick)
            {
                if (nick.empty())
                {
                    m_gui->get_element<GUI::TextRenderer>("m1")->set_text("Fill nickname");
                }
                else
                {
                    m_nickname = nick;
                }
            });

    m_gui->add_element<GUI::TextRenderer>(multmenu, ResourceManager::get_font("calibri"), ResourceManager::getShaderProgram("textShader"),
        "Enter IP", glm::vec3(1.f), glm::vec2(88.f, 68.f), glm::vec2(1.f), "m");

    m_gui->add_element<GUI::InputField>(multmenu, new GUI::Sprite(ResourceManager::getMaterial("default_input"), "static"),
        glm::vec2(88.f, 60.f), glm::vec2(10.f, 5.f), "InputIP", ResourceManager::getShaderProgram("textShader"),
        ResourceManager::get_font("calibri"), glm::vec3(1.f), false, lock_key_update);

    m_gui->add_element<GUI::Button>(multmenu, new GUI::Sprite(ResourceManager::getMaterial("button"), "static"),
        glm::vec2(88.f, 48.f), glm::vec2(10.f, 5.f),
        "Disconnect", ResourceManager::getShaderProgram("textShader"), ResourceManager::get_font("calibri"), glm::vec3(1.f));

    m_gui->add_element<GUI::CheckBox>(multmenu, new GUI::Sprite(ResourceManager::getMaterial("checkbox_bg")), new GUI::Sprite(ResourceManager::getMaterial("checkbox_mark")),
        glm::vec2(70.f, 60.f), glm::vec2(5.f), "checkbox")->set_click_callback([&]() {
            if (m_gui->get_element<GUI::CheckBox>("checkbox")->value())
            {
                m_gui->get_element<GUI::InputField>("InputIP")->set_text("0.0.0.0");
            }
            else
            {
                m_gui->get_element<GUI::InputField>("InputIP")->set_text("127.0.0.1");
            }
            });

    multmenu->set_active(false);

    m_gui->get_element<GUI::GUI_element>("Disconnect")->set_click_callback([&]() {
        WinSock::disconnect();
        m_chat_mes.push("Disconnect!");
        });

    m_gui->get_element<GUI::InputField>("InputIP")->set_text("127.0.0.1");

    m_gui->get_element<GUI::InputField>("InputIP")->set_enter_callback([&](std::string text)
        {
            if (m_nickname.empty())
            {
                m_chat_mes.push("Enter nickname!");
                return;
            }
            //m_gui_chat->get_element<GUI::ChatBox>("Chat")->clear(); 
            WinSock::InitCodes code;
            if (m_gui->get_element<GUI::CheckBox>("checkbox")->value())
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

    m_gui->sort_render_list();
}

void GameApp::start_game_single()
{
    map.fill(false);
    isKeyPressed = false;
    m_isLose = false;
    countKills = 0;
    countEnemies = 0; 
    countEnemiesPerm = 0;
    cur = 0;
    //int((size_x * size_y) / 2) + int(size_x / 2)
    m_main_castle = new Castle(parts[86], _set_max_hp_castle,
        ResourceManager::get_OBJ_model("castle"), ResourceManager::getMaterial("castle"), ResourceManager::getMaterial("default"));
    map[86] = true;

    for (size_t i = 0; i < m_towers.size(); i++)
    {
        delete m_towers[i].tow;
    }
    for (size_t i = 0; i < m_enemies.size(); i++)
    {
        delete m_enemies[i];
    }

    m_towers.clear();
    m_enemies.clear();
    
    is_gui_active = false;

    is_spawn_mode = false;
    is_spawn_enemy = false;
    m_scene.at(curObj)->getComponent<Highlight>()->set_color(glm::vec3(1.f));

    m_gui->get_element<GUI::TextRenderer>("enemies")->set_text("Enemies: 0");
    m_gui->get_element<GUI::TextRenderer>("kills")->set_text("Kills: 0");

    m_gui->get_element<GUI::GUI_element>("lose_place")->set_active(false);
    m_gui->get_element<GUI::GUI_element>("settings_place")->set_active(false);
    m_gui->get_element<GUI::GUI_element>("menu_place")->set_active(false);

    // crutch, but now is the best solve (reset settings when connect)
    //_set_min_distance = 7;
    //_set_velosity = 7;
    //_set_max_hp_castle = 100;
    //_set_max_hp_enemy = 50;
    //_set_cooldown_tower = 3;
    //_set_damage_tower = 10;
}

void GameApp::start_game_multi()
{
    //map.fill(false);
    //isKeyPressed = false;
    //m_isLose = false;
    //countKills = 0;
    //countEnemies = 0;
    //countEnemiesPerm = 0;
    //cur = 0;

    //for (size_t i = 0; i < m_towers.size(); i++)
    //{
    //    delete m_towers[i];
    //}
    //for (size_t i = 0; i < m_towers_self.size(); i++)
    //{
    //    delete m_towers_self[i];
    //}
    //for (size_t i = 0; i < m_enemies.size(); i++)
    //{
    //    delete m_enemies[i];
    //}
    //for (size_t i = 0; i < m_enemies_self.size(); i++)
    //{
    //    delete m_enemies_self[i];
    //}
    //m_towers.clear();
    //m_enemies.clear();

    //m_towers_self.clear();
    //m_enemies_self.clear();

    //is_gui_active = false;

    //is_spawn_mode = false;
    //is_spawn_enemy = false;
    //m_scene.at(curObj)->getComponent<Highlight>()->set_color(glm::vec3(1.f));

    //m_gui->get_element<GUI::TextRenderer>("enemies")->set_text("Enemies: 0");
    //m_gui->get_element<GUI::TextRenderer>("kills")->set_text("Kills: 0");
    ////m_gui_place_menu->get_element<GUI::InputField>("InputIP")->set_focus(false);

    //m_gui->set_active(false);
    ////m_gui_place_settings->set_active(false);

    ////m_adv_castle = new Castle(parts[int(size_x * (size_y / 8)) + int(size_x / 4)], _set_max_hp_castle,
    ////    ResourceManager::load_OBJ_file("res/models/castle.obj"), ResourceManager::getMaterial("castle"), ResourceManager::getMaterial("default"));
    ////map[int(size_x * (size_y / 8)) + int(size_x / 8)] = true;

    ////m_main_castle = new Castle(parts[int(size_x * ((size_y * 7) / 8)) + int((size_x * 3) / 4)], _set_max_hp_castle,
    ////    ResourceManager::load_OBJ_file("res/models/castle.obj"), ResourceManager::getMaterial("castle"), ResourceManager::getMaterial("default"), glm::vec3(0.f, 0.f, 1.f));
    ////map[int(size_x * ((size_y * 7) / 8)) + int((size_x * 7) / 8)] = true;

    //if (isServer)
    //{
    //    m_cam->set_position_rotation(glm::vec3(12.5f, 55.f, 30.f), glm::vec3(-75.f, -90.f, 0.f));
    //}   
    //else
    //{
    //    m_cam->set_position_rotation(glm::vec3(45.5f, 55.f, 30.f), glm::vec3(-75.f, -270.f, 0.f));
    //}
}

void GameApp::init_winSock()
{
    WinSock::init_WinSock();

    WinSock::set_receive_callback([&](char* data, int size) {
        if (data[WS_DATA_PACKET_INFO_SIZE] == 'm') // message for chat
        {
            std::string str = std::string(&data[WS_DATA_PACKET_INFO_SIZE + 1]).substr(0, size - (WS_DATA_PACKET_INFO_SIZE));
            //m_chat_mes.push(m_nickname_connect + ": " + str);
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
            restart_querry = true;
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
            //m_nickname_connect = str;
            std::string temp = isServer ? ": " : "to: ";
            m_chat_mes.push("Connect " + temp + str);
        }
        else if (data[WS_DATA_PACKET_INFO_SIZE] == 'c')
        {
            /*if (data[WS_DATA_PACKET_INFO_SIZE + 1] == 'a')
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
            }*/
        }
        });

    WinSock::set_ping_callback([&](double ping) {
        m_gui->get_element<GUI::TextRenderer>("ping")->set_text(std::to_string((float)ping) + " ms");
        });

    WinSock::set_disconnect_callback([&]() {
        m_chat_mes.push("Disconnect!");
        m_scene.at(4)->getComponent<Highlight>()->set_active(false);
        m_gui->get_element<GUI::Button>("Disconnect")->set_active(false);
        m_gui->get_element<GUI::Button>("RestartMenu")->set_active(true);
        m_gui->get_element<GUI::Button>("Restart")->set_active(true);
        });
    WinSock::set_connect_callback([&]() {
        WinSock::send_data(('n' + m_nickname).data(), m_nickname.length() + 1); // =========================== message flag first
        m_scene.at(4)->getComponent<Highlight>()->set_active(true);
        m_gui->get_element<GUI::Button>("Disconnect")->set_active(true);
        if (!isServer)
        {
            m_gui->get_element<GUI::Button>("RestartMenu")->set_active(false);
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
        m_mode = GameMode::Multi;
        restart_querry = true;
        });
}

void GameApp::add_green_place(glm::vec3 startPos, glm::vec3 endPos)
{
    glm::vec3 pos((endPos.x - startPos.x) / 2.f, 0.1f, (endPos.z - startPos.z) / 2.f);
    m_scene.add_object<Plane>(ResourceManager::getMaterial("green_place"), glm::vec3(startPos.x, 0.f, startPos.z) + pos, pos);
}

void INIregionUSER::parse(std::string name, std::string value)
{
    if (name == "nickname") *nickname = value;
}

std::string INIregionUSER::get_str_data()
{
    std::string data = "";
    data += "nickname=" + *nickname + "\n";
    return data;
}
