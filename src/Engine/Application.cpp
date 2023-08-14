#include "EngineCore/Application.h"
#include "EngineCore/Window.h"
#include "EngineCore/Input.h"

#include "EngineCore/System/Log.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/Sprite.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <chrono>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

const GLfloat vertexCoords[] = {
    // FRONT
    -1.f, -1.f, -1.f,
    -1.f,  1.f, -1.f,
    -1.f,  1.f,  1.f,
    -1.f, -1.f,  1.f,
    // BACK
    1.f, -1.f, -1.f,
    1.f,  1.f, -1.f,
    1.f,  1.f,  1.f,
    1.f, -1.f,  1.f,
    // RIGHT
    -1.f,  1.f, -1.f,
     1.f,  1.f, -1.f,
     1.f,  1.f,  1.f,
    -1.f,  1.f,  1.f,
    // LEFT
    -1.f, -1.f, -1.f,
     1.f, -1.f, -1.f,
     1.f, -1.f,  1.f,
    -1.f, -1.f,  1.f,
    // TOP
    -1.f, -1.f,  1.f,
    -1.f,  1.f,  1.f,
     1.f,  1.f,  1.f,
     1.f, -1.f,  1.f,
    // BOTTOM
    -1.f, -1.f, -1.f,
    -1.f, -1.f, -1.f,
     1.f, -1.f, -1.f,
     1.f, -1.f, -1.f
};

const GLfloat textureCoords[] = {
    // FRONT
    0.f, 0.f,
    1.f, 0.f, 
    1.f, 1.f,
    0.f, 1.f,
    // BACK
    1.f, 0.f, 
    0.f, 0.f,  
    0.f, 1.f, 
    1.f, 1.f,
    // RIGHT
    0.f, 0.f,
    1.f, 0.f,
    1.f, 1.f,
    0.f, 1.f,
    // LEFT
    1.f, 0.f,
    0.f, 0.f,
    0.f, 1.f,
    1.f, 1.f,
    // TOP
    0.f, 0.f,
    1.f, 0.f,
    1.f, 1.f,
    0.f, 1.f,
    // BOTTOM
    1.f, 0.f,
    0.f, 0.f,
    0.f, 1.f,
    1.f, 1.f,
};

const GLfloat normalCoords[] = {
    // FRONT
    -1.f, 0.f, 0.f,
    -1.f, 0.f, 0.f,
    -1.f, 0.f, 0.f,
    -1.f, 0.f, 0.f,
    // BACK
    1.f, 0.f, 0.f,
    1.f, 0.f, 0.f,
    1.f, 0.f, 0.f,
    1.f, 0.f, 0.f,
    // RIGHT
    0.f,  1.f, 0.f,
    0.f,  1.f, 0.f,
    0.f,  1.f, 0.f,
    0.f,  1.f, 0.f,
    // LEFT
    0.f, -1.f, 0.f,
    0.f, -1.f, 0.f,
    0.f, -1.f, 0.f,
    0.f, -1.f, 0.f,
    // TOP
    0.f, 0.f,  1.f,
    0.f, 0.f,  1.f,
    0.f, 0.f,  1.f,
    0.f, 0.f,  1.f,
     // BOTTOM
     0.f, 0.f, -1.f,
     0.f, 0.f, -1.f,
     0.f, 0.f, -1.f,
     0.f, 0.f, -1.f
};

GLuint indexes[]
{
    //front
    0, 1, 2, 2, 3, 0,
    // back
    4, 5, 6, 6, 7, 4,
    // right
    8, 9, 10, 10, 11, 8,
    // left
    12, 13, 14, 14, 15, 12,
    // top
    16, 17, 18, 18, 19, 16,
    // bottom
    20, 21, 22, 22, 23, 20
};

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
    m_cam = new Camera(glm::vec3(0), glm::vec3(0));

    m_pCloseWindow = false;
    m_pWindow = std::make_unique<Window>(title, window_size);
    m_cam->set_viewport_size(static_cast<float>(window_size.x), static_cast<float>(window_size.y));

    m_event_dispather.add_event_listener<EventWindowResize>([&](EventWindowResize& e)
        {
            LOG_INFO("[EVENT] Resize: {0}x{1}", e.width, e.height);
            RenderEngine::Renderer::setViewport(e.width, e.height);
            m_cam->set_viewport_size(e.width, e.height);
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
    m_event_dispather.add_event_listener<EventMouseButtonPressed>([&](EventMouseButtonPressed& e)
        {
            LOG_INFO("[EVENT]: Mouse button pressed at ({0}x{1})", e.x_pos, e.y_pos);
            Input::pressMouseButton(e.mouse_button);
            on_button_mouse_event(e.mouse_button, e.x_pos, e.y_pos, true);
        }); 
    m_event_dispather.add_event_listener<EventMouseButtonReleased>([&](EventMouseButtonReleased& e)
        {
            LOG_INFO("[EVENT]: Mouse button released at ({0}x{1})", e.x_pos, e.y_pos);
            Input::releaseMouseButton(e.mouse_button);
            on_button_mouse_event(e.mouse_button, e.x_pos, e.y_pos, false);
        });
    m_pWindow->set_event_callback(
        [&](BaseEvent& e)
        {
            m_event_dispather.dispatch(e);
        });

    ResourceManager::loadJSONresources("res/resources.json");

    //auto pShapeProgram = ResourceManager::getShaderProgram("shapeShader");

    //m_line = new RenderEngine::Line(pShapeProgram);

    // --------------------------------------------------------- //
    m_pTextureAtlas = ResourceManager::getTexture("CubeTexture");
    m_pShaderProgram = ResourceManager::getShaderProgram("shape3DShader");

    m_vertexArray = std::make_shared<RenderEngine::VertexArray>();

    m_vertexCoordsBuffer.init(&vertexCoords, 24 * 3 * sizeof(GLfloat));
    RenderEngine::VertexBufferLayout vertexCoordsLayout;
    vertexCoordsLayout.addElementLayoutFloat(3, false);
    m_vertexArray->addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

    m_vertexNormalBuffer.init(&normalCoords, 24 * 3 * sizeof(GLfloat));
    RenderEngine::VertexBufferLayout vertexNormalLayout;
    vertexNormalLayout.addElementLayoutFloat(3, false);
    m_vertexArray->addBuffer(m_vertexNormalBuffer, vertexNormalLayout);

    m_textureCoordsBuffer.init(&textureCoords, 24 * 2 * sizeof(GLfloat));
    RenderEngine::VertexBufferLayout textureCoordsLayout;
    textureCoordsLayout.addElementLayoutFloat(2, false);
    m_vertexArray->addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

    m_indexBuffer.init(&indexes, sizeof(indexes) / sizeof(GLuint));

    m_vertexArray->unbind();
    m_indexBuffer.unbind();
    // --------------------------------------------------------- //

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

        glm::vec3 pos(0), rot(0);

        pos = m_cam->get_position();
        rot = m_cam->get_rotation();

        m_cam_pos[0] = pos.x;
        m_cam_pos[1] = pos.y;
        m_cam_pos[2] = pos.z;

        m_cam_rot[0] = rot.x;
        m_cam_rot[1] = rot.y;
        m_cam_rot[2] = rot.z;

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(m_pWindow->get_size().x);
        io.DisplaySize.y = static_cast<float>(m_pWindow->get_size().y);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Background Color Window");
        ImGui::ColorEdit4("Background Color", m_colors);
        ImGui::SliderFloat3("Sprite position", m_sprite_pos, -50.f, 50.f);
        ImGui::SliderFloat3("Cube position", m_cube_pos, -50.f, 50.f);
        ImGui::SliderFloat3("Cube scale", m_cube_scale, -50.f, 50.f);
        if (ImGui::SliderFloat3("Camera position", m_cam_pos, -50.f, 50.f))
        {
            m_cam->set_position(glm::vec3(m_cam_pos[0], m_cam_pos[1], m_cam_pos[2]));
        }
        if (ImGui::SliderFloat3("Camera rotation", m_cam_rot, 0.f, 360.f))
        {
            m_cam->set_rotation(glm::vec3(m_cam_rot[0], m_cam_rot[1], m_cam_rot[2]));
        }
        if (ImGui::SliderFloat("Camera fov", &m_cam_fov, 1.f, 120.f))
        {
            m_cam->set_field_of_view(m_cam_fov);
        }
        ImGui::SliderFloat("Sensetivity", &m_cam_sensetivity, 0.001f, 1.f);
        ImGui::SliderFloat("Addition speed", &m_add_ctrl_speed, 1.f, 4.f);
        if (ImGui::Checkbox("Perspective camera", &m_isPerspectiveCam))
        {
            m_cam->set_projection_mode(m_isPerspectiveCam ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
        }
        ImGui::Checkbox("Inversive mouse", &m_isInversiveMouseY);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        ResourceManager::getShaderProgram("shapeShader")->setMatrix4("view_projectionMat", m_cam->get_projection_matrix() * m_cam->get_view_matrix());
        ResourceManager::getShaderProgram("shape3DShader")->setMatrix4("view_projectionMat", m_cam->get_projection_matrix()* m_cam->get_view_matrix());
        ResourceManager::getShaderProgram("spriteShader")->setMatrix4("view_projectionMat", m_cam->get_projection_matrix() * m_cam->get_view_matrix());

        //ResourceManager::getSprite("TankSprite")->render(glm::vec3(m_sprite_pos[0], m_sprite_pos[1], m_sprite_pos[2]), glm::vec3(5), 0);

        //m_line->render(glm::vec3(0.f), glm::vec3(-10.f), glm::vec3(1.f));

        // --------------------------------------------------------- //
        m_pShaderProgram->use();

        glm::mat4 scaleMat(
            m_cube_scale[0], 0, 0, 0,
            0, m_cube_scale[1], 0, 0,
            0, 0, m_cube_scale[2], 0,
            0, 0, 0, 1);

        glm::mat4 translateMat(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            m_cube_pos[0], m_cube_pos[1], m_cube_pos[2], 1);

        glm::mat4 model = translateMat * scaleMat;
        m_pShaderProgram->setMatrix4("modelMat", model);

        RenderEngine::Renderer::bindTexture(*m_pTextureAtlas);
        RenderEngine::Renderer::drawTriangles(*m_vertexArray, m_indexBuffer, *m_pShaderProgram);

        glm::mat4 translateMatA(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            m_cube_pos[0] + 5, m_cube_pos[1], m_cube_pos[2], 1);

        model = translateMatA * scaleMat;
        m_pShaderProgram->setMatrix4("modelMat", model);

        RenderEngine::Renderer::drawTriangles(*m_vertexArray, m_indexBuffer, *m_pShaderProgram);
        // --------------------------------------------------------- //

        m_pWindow->on_update();
        on_update(duration);
    }

    m_pWindow = nullptr;

    return 0;
}

void Application::on_update(const double delta)
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
        movement_delta.x += static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_S))
    {
        movement_delta.x -= static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_A))
    {
        movement_delta.y -= static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_D))
    {
        movement_delta.y += static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_SPACE))
    {
        movement_delta.z += static_cast<float>(addSpeed * m_cam_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_LEFT_SHIFT))
    {
        movement_delta.z -= static_cast<float>(addSpeed * m_cam_velocity * delta);
    }

    else if (Input::isKeyPressed(KeyCode::KEY_UP))
    {
        rotation_delta.y += static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_DOWN))
    {
        rotation_delta.y -= static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_LEFT))
    {
        rotation_delta.z -= static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_RIGHT))
    {
        rotation_delta.z += static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_Q))
    {
        rotation_delta.x += static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_E))
    {
        rotation_delta.x -= static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }

    if (Input::isMouseButtonPressed(MouseButton::MOUSE_BUTTON_RIGHT))
    {
       glm::vec2 pos = m_pWindow->get_current_cursor_position();
       rotation_delta.z = m_isInversiveMouseY 
           ? (rotation_delta.z + ((m_init_mouse_pos_x - pos.x) / (1 / m_cam_sensetivity)))
           : (rotation_delta.z - ((m_init_mouse_pos_x - pos.x) / (1 / m_cam_sensetivity)));
       rotation_delta.y += (m_init_mouse_pos_y - pos.y) / (1 / m_cam_sensetivity);
       m_init_mouse_pos_x = pos.x;
       m_init_mouse_pos_y = pos.y;
    }

    m_cam->add_movement_and_rotation(movement_delta, rotation_delta);
}

void Application::on_button_mouse_event(const MouseButton button, const double pos_x, const double pos_y, const bool isPressed)
{
    m_init_mouse_pos_x = pos_x;
    m_init_mouse_pos_y = pos_y;
}
