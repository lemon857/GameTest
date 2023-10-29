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
#include "EngineCore/Meshes/ObjModel.h"
#include "EngineCore/Meshes/EmptyObject.h"
#include "EngineCore/Meshes/Plane.h"
#include "EngineCore/Meshes/Grid.h" 

#include "EngineCore/GUI/GUI_place.h"
#include "EngineCore/GUI/Square.h"

#include <array>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int size_x = 50, size_y = 50;

std::array < bool, size_x* size_y> map;

int cur = 0;
int curObj = 3;

// --------------------------------------------------------------------- //

unsigned int VAO1, VBO1;
void RenderText(std::shared_ptr<RenderEngine::ShaderProgram> s, std::string text, float x, float y, float scale, glm::vec3 color, glm::mat4& prj)
{
    // activate corresponding render state	
    s->use();
    s->setVec3("textColor", glm::vec3(color.x, color.y, color.z));
    s->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, prj);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO1);

    // iterate through all Characters1
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Font_Glyph ch = ResourceManager::get_character("agaaler", *c);

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO1);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// --------------------------------------------------------------------- //
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

    m_gui_place = new GUI::GUI_place(m_cam, ResourceManager::getMaterial("default"));

    auto square = new GUI::Square(ResourceManager::getMaterial("default"), glm::vec2(100.f), glm::vec2(100.f));

    square->set_click_callback([]() 
        {
            LOG_INFO("EEE HELLO WORLD YEPTA");
        });

    m_gui_place->add_element(square);

    m_line = new RenderEngine::Line(ResourceManager::getMaterial("default"));
    
    map.fill(false);

	std::vector<std::string> names;

	names.push_back("default3DShader");

	m_scene.add_object<Plane>(ResourceManager::getMaterial("cube"), glm::vec3(size_x, 0.f, size_y), glm::vec3(size_x, 0.f, size_y));
    m_scene.add_object<DirectionalLight>(names);
    m_scene.add_object<Grid>(glm::vec3(size_x, 0.5f, size_y), glm::vec2(1.f), size_x, size_y, glm::vec3(1.f), ResourceManager::getMaterial("default"));
    m_scene.add_object<ObjModel>("res/models/monkey.obj", ResourceManager::getMaterial("monkey"));
    //m_scene.add_object<Cube>(ResourceManager::getMaterial("cube"));

    ((float*)ResourceManager::getMaterial("cube")->get_data("ambient_factor"))[0] = 0.3f;
    ((float*)ResourceManager::getMaterial("monkey")->get_data("ambient_factor"))[0] = 0.2f;

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


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	return true;
}
// цикл для проерки нажатия клавиш
void GameApp::on_key_update(const double delta)
{
    glm::vec3 movement_delta{ 0,0,0 };
    glm::vec3 rotation_delta{ 0,0,0 };

    double addSpeed = 1;

    //if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
    //{
    //    int x = (int)floor(m_world_mouse_pos_x / 2.f);
    //    int y = (int)floor(m_world_mouse_pos_z / 2.f);

    //    if (x * y <= parts.size())
    //    {
    //        cur = x * size_y + y;
    //    }
    //}
    //else if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_RIGHT))
    //{
    //    if (!map[cur])
    //    {
    //        map[cur] = true;
    //        m_scene.at(curObj)->deleteComponent<Highlight>();
    //        curObj++;

    //        m_scene.add_object<ObjModel>("res/models/monkey.obj", ResourceManager::getMaterial("monkey"));
    //        //m_scene.add_object<Cube>(ResourceManager::getMaterial("cube"));

    //        m_scene.at(curObj)->addComponent<Transform>();
    //        m_scene.at(curObj)->addComponent<Highlight>(ResourceManager::getMaterial("default"), true);
    //        LOG_INFO("Add object");
    //    }
    //}

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
    ResourceManager::getShaderProgram("colorShader")->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_cam->get_projection_matrix() * m_cam->get_view_matrix());
    //ResourceManager::getShaderProgram("colorShader")->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_cam->get_ui_matrix());

    ResourceManager::getShaderProgram("default3DShader")->use();
    ResourceManager::getShaderProgram("default3DShader")->setVec3("cam_position", m_cam->get_position());

    m_gui_place->on_update(delta);

    m_scene.at(curObj)->getComponent<Transform>()->set_position(parts[cur]);  

    for (size_t i = 0; i < m_scene.get_list().size(); i++)
    {
        MeshRenderer* mesh = m_scene.at(i)->getComponent<MeshRenderer>();
        if (mesh != nullptr)
        {
            std::shared_ptr<RenderEngine::ShaderProgram> shader = mesh->get_material_ptr()->get_shader_ptr();
            shader->use();
            shader->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_cam->get_projection_matrix() * m_cam->get_view_matrix());
        }
        m_scene.at(i)->update(delta);
    }
}
// отрисовка интерфейса
void GameApp::on_ui_render()
{
    //m_funcUpdate();
    RenderText(ResourceManager::getShaderProgram("textShader"), "Hello, world!", 500.f, 500.f, 2.f, glm::vec3(1.f, 0.f, 0.f), m_cam->get_ui_matrix());
    m_gui_place->on_render();
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

            m_mouse_pos_x = e.x;
            m_mouse_pos_y = e.y;

            LOG_INFO("[EVENT] Mouse moved to {0}x{1}", e.x, e.y);
        });
    m_event_dispather.add_event_listener<EventMouseScrolled>(
        [&](EventMouseScrolled& e)
        {
            m_gui_place->get_element(0)->add_position(glm::vec2(0.f, e.y_offset * 2.f));
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
            m_gui_place->on_mouse_click(e.x_pos, e.y_pos);
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
