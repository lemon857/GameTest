#include "EngineCore/Application.h"
#include "EngineCore/Window.h"
#include "EngineCore/Input.h"

#include "EngineCore/System/Log.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Components/SpriteRenderer.h"
#include "EngineCore/Renderer3D/GraphicsObject.h"
#include "EngineCore/Components/Transform.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <chrono>

#include <glm/gtc/matrix_transform.hpp>

const GLfloat vertexCoords[] = {
    // FRONT
    -1.f, -1.f, -1.f,//1
    -1.f,  1.f, -1.f,//2
     1.f,  1.f,  -1.f,//3
     1.f, -1.f,  -1.f,//4
    // BACK
    1.f, -1.f, 1.f,//5
    1.f,  1.f, 1.f,//6
    -1.f,  1.f,  1.f,//7
    -1.f, -1.f,  1.f,//8
    // RIGHT
    1.f,  -1.f, -1.f,//9
     1.f,  1.f, -1.f,//10
     1.f,  1.f,  1.f,//11
    1.f,  -1.f,  1.f,//12
    // LEFT
    -1.f, -1.f, 1.f,//13
    -1.f,  1.f, 1.f,//14
    -1.f,  1.f, -1.f,//15
    -1.f, -1.f, -1.f,//16
    // TOP
    -1.f,  1.f,  -1.f,//17
    -1.f,  1.f,  1.f,//18
     1.f,  1.f,  1.f,//19
     1.f,  1.f,  -1.f,//20
    // BOTTOM
    1.f, -1.f, -1.f,//21
    1.f, -1.f, 1.f,//22
    -1.f, -1.f, 1.f,//23
    -1.f, -1.f, -1.f //24
};

const GLfloat textureCoords[] = {
    // FRONT
    1.f, 0.f,
    1.f, 1.f,
    0.f, 1.f,
    0.f, 0.f,
    // BACK
    1.f, 0.f,
    1.f, 1.f,
    0.f, 1.f,
    0.f, 0.f,
    // RIGHT
    1.f, 0.f,
    1.f, 1.f,
    0.f, 1.f,
    0.f, 0.f,
    // LEFT
    1.f, 0.f,
    1.f, 1.f,
    0.f, 1.f,
    0.f, 0.f,
    // TOP
    1.f, 0.f,
    1.f, 1.f,
    0.f, 1.f,
    0.f, 0.f,
    // BOTTOM
    1.f, 0.f,
    1.f, 1.f,
    0.f, 1.f,
    0.f, 0.f,
};

const GLfloat normalCoords[] = {
    // FRONT // 4
    0.f, 0.f, -1.f,
    0.f, 0.f, -1.f,
    0.f, 0.f, -1.f,
    0.f, 0.f, -1.f,
    // BACK // 3
    0.f, 0.f, 1.f,
    0.f, 0.f, 1.f,
    0.f, 0.f, 1.f,
    0.f, 0.f, 1.f,
    // RIGHT // 2
    1.f,  0.f, 0.f,
    1.f,  0.f, 0.f,
    1.f,  0.f, 0.f,
    1.f,  0.f, 0.f,
    // LEFT // 1
    -1.f, 0.f, 0.f,
    -1.f, 0.f, 0.f,
    -1.f, 0.f, 0.f,
    -1.f, 0.f, 0.f,
    // TOP // 6
    0.f, 1.f,  0.f,
    0.f, 1.f,  0.f,
    0.f, 1.f,  0.f,
    0.f, 1.f,  0.f,
     // BOTTOM // 5
     0.f, -1.f, 0.f,
     0.f, -1.f, 0.f,
     0.f, -1.f, 0.f,
     0.f, -1.f, 0.f
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
    INIdata data{ window_size, m_window_position, m_maximized_window };
    ResourceManager::loadINIsettings("EngineTest.ini", data, false);

    m_cam = new Camera(glm::vec3(0), glm::vec3(0));

    //m_ray = new Ray();

    m_pCloseWindow = false;
    m_pWindow = std::make_unique<Window>(title, m_window_position, window_size, m_maximized_window);

    m_cam->set_viewport_size(static_cast<float>(window_size.x), static_cast<float>(window_size.y));

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

    ResourceManager::loadJSONresources("res/resources.json");

    auto pShapeProgram = ResourceManager::getShaderProgram("shapeShader");
    auto pSpriteProgram = ResourceManager::getShaderProgram("spriteShader");

    m_line = new RenderEngine::Line(pShapeProgram);

    m_cube = new Cube(ResourceManager::getShaderProgram("shape3DShader"), ResourceManager::getTexture("WallAtlas"));

    m_cube->addComponent<Transform>();
          
    m_cube->getComponent<Transform>()->set_position(glm::vec3(m_SpriteRenderer_pos[0], m_SpriteRenderer_pos[1], m_SpriteRenderer_pos[2]));
    m_cube->getComponent<Transform>()->set_scale(glm::vec3(m_SpriteRenderer_scale[0], m_SpriteRenderer_scale[1], m_SpriteRenderer_scale[2]));
    m_cube->getComponent<Transform>()->set_rotation(glm::vec3(m_SpriteRenderer_rot[0], m_SpriteRenderer_rot[1], m_SpriteRenderer_rot[2]));
    m_cube->setSubTexture("BrickWall");
    // --------------------------------------------------------- //
    m_pTextureAtlas = ResourceManager::getTexture("CubeTexture");
    m_pShaderProgram_light = ResourceManager::getShaderProgram("lightSourceShader");
    m_pShaderProgram = ResourceManager::getShaderProgram("shape3DShader");

    m_vertexArray = std::make_shared<RenderEngine::VertexArray>();

    m_vertexCoordsBuffer.init(&vertexCoords, 24 * 3 * sizeof(GLfloat), false);
    RenderEngine::VertexBufferLayout vertexCoordsLayout;
    vertexCoordsLayout.addElementLayoutFloat(3, false);
    m_vertexArray->addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

    m_vertexNormalBuffer.init(&normalCoords, 24 * 3 * sizeof(GLfloat), false);
    RenderEngine::VertexBufferLayout vertexNormalLayout;
    vertexNormalLayout.addElementLayoutFloat(3, false);
    m_vertexArray->addBuffer(m_vertexNormalBuffer, vertexNormalLayout);

    m_textureCoordsBuffer.init(&textureCoords, 24 * 2 * sizeof(GLfloat), false);
    RenderEngine::VertexBufferLayout textureCoordsLayout;
    textureCoordsLayout.addElementLayoutFloat(2, false);
    m_vertexArray->addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

    m_indexBuffer.init(&indexes, sizeof(indexes) / sizeof(GLuint));

    m_vertexArray->unbind();
    m_vertexArray_light = std::make_shared<RenderEngine::VertexArray>();

    m_vertexArray_light->addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

    m_vertexArray_light->unbind();
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

        ResourceManager::getShaderProgram("shapeShader")->use();
        ResourceManager::getShaderProgram("shapeShader")->setMatrix4("view_projectionMat", m_cam->get_projection_matrix() * m_cam->get_view_matrix());
        ResourceManager::getShaderProgram("shape3DShader")->use();
        ResourceManager::getShaderProgram("shape3DShader")->setMatrix4("view_projectionMat", m_cam->get_projection_matrix() * m_cam->get_view_matrix());
        ResourceManager::getShaderProgram("spriteShader")->use();
        ResourceManager::getShaderProgram("spriteShader")->setMatrix4("view_projectionMat", m_cam->get_projection_matrix() * m_cam->get_view_matrix());
        ResourceManager::getShaderProgram("lightSourceShader")->use();
        ResourceManager::getShaderProgram("lightSourceShader")->setMatrix4("view_projectionMat", m_cam->get_projection_matrix() * m_cam->get_view_matrix());

        m_line->render(glm::vec3(0.f), glm::vec3(m_line_pos[0], m_line_pos[1], m_line_pos[2]), glm::vec3(1.f));

        //m_line->render_from_to(m_cam->get_position() - glm::vec3(1.f), glm::vec3(m_world_mouse_pos_x, m_world_mouse_pos_y, m_world_mouse_pos_z), glm::vec3(1.f));

        //m_line->render_from_to(glm::vec3(0.f), glm::vec3(m_world_mouse_pos_x, m_world_mouse_pos_y, m_world_mouse_pos_z), glm::vec3(1.f));

        if (m_drawNullIntersection)
        {
            m_line->render_from_to(glm::vec3(0.f, 0.f, 50.f), glm::vec3(0.f, 0.f, -50.f), glm::vec3(0.f, 0.f, 1.f));
            m_line->render_from_to(glm::vec3(0.f, 50.f, 0.f), glm::vec3(0.f, -50.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
            m_line->render_from_to(glm::vec3(50.f, 0.f, 0.f), glm::vec3(-50.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
        }

        // --------------------------------------------------------- //
        
        // Cube

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

        float xRotRadians = glm::radians(m_cube_rot[0]);
        float yRotRadians = glm::radians(m_cube_rot[1]);
        float zRotRadians = glm::radians(m_cube_rot[2]);

        glm::mat4 rotateXmat(
            1, 0, 0, 0,
            0, cos(xRotRadians), -sin(xRotRadians), 0,
            0, sin(xRotRadians), cos(xRotRadians), 0,
            0, 0, 0, 1);

        glm::mat4 rotateYmat(
            cos(yRotRadians), 0, -sin(yRotRadians), 0,
            0, 1, 0, 0,
            sin(yRotRadians), 0, cos(yRotRadians), 0,
            0, 0, 0, 1);

        glm::mat4 rotateZmat(
            cos(zRotRadians), -sin(zRotRadians), 0, 0,
            sin(zRotRadians), cos(zRotRadians), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);

        glm::mat4 model =  translateMat * rotateXmat * rotateYmat * rotateZmat * scaleMat;

        m_pShaderProgram->setVec3("light_color", glm::vec3(m_light_color[0], m_light_color[1], m_light_color[2]));
        m_pShaderProgram->setVec3("light_position", glm::vec3(m_light_pos[0], m_light_pos[1], m_light_pos[2]));
        m_pShaderProgram->setVec3("cam_position", m_cam->get_position());
        m_pShaderProgram->setFloat("ambient_factor", m_ambient_factor);
        m_pShaderProgram->setFloat("diffuse_factor", m_diffuse_factor);
        m_pShaderProgram->setFloat("specular_factor", m_specular_factor);
        m_pShaderProgram->setFloat("shininess", m_shininess);
        m_pShaderProgram->setInt("isMetalic", m_isMetalic);
        //m_pShaderProgram->setVec4("sourceColor", glm::vec4(1.f));
        m_pShaderProgram->setMatrix4("modelMat", model);

        RenderEngine::Renderer::bindTexture(*m_pTextureAtlas);
        RenderEngine::Renderer::drawTriangles(*m_vertexArray, m_indexBuffer);

        // Light source

        glm::mat4 translateMatLight(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            m_light_pos[0], m_light_pos[1], m_light_pos[2], 1);

        m_pShaderProgram_light->use();
        m_pShaderProgram_light->setMatrix4("modelMat", translateMatLight);
        m_pShaderProgram_light->setVec3("light_color", glm::vec3(m_light_color[0], m_light_color[1], m_light_color[2]));
        RenderEngine::Renderer::drawTriangles(*m_vertexArray_light, m_indexBuffer);

        // --------------------------------------------------------- //

        m_cube->update(duration);

        // ------------------------------------------------------------ // 
        on_ui_render();
        // ------------------------------------------------------------ // 
        m_pWindow->on_update();
        on_key_update(duration);
    }

    INIdata endData{ m_pWindow->get_size(), m_window_position, m_maximized_window};
    ResourceManager::loadINIsettings("EngineTest.ini", endData, true);
    m_pWindow = nullptr;

    return 0;
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
        rotation_delta.x -= static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_DOWN))
    {
        rotation_delta.x += static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
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
        rotation_delta.z -= static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_E))
    {
        rotation_delta.z += static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
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
