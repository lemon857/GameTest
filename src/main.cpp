//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#include <glm/vec2.hpp>

#include <iostream>
#include <chrono>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "EngineCore/Window.h"
#include "EngineCore/UImodule.h"

#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"
#include "Examples/TanksDemo/TanksDemoGame.h"
#include "Examples/SnakeDemo/SnakeDemoGame.h"
#include "Examples/PongDemo/PongDemoGame.h"

#include "EngineCore/Physics/PhysicsEngine.h"
#include "EngineCore/Application.h"
#include "EngineCore/System/Log.h"

char* b;
class MyApp : public Application
{
public:
    MyApp()
    {
        b = new char[3] { 'c', 'a', 'n' };
    }
    ~MyApp()
    {

    }
    void on_ui_render() override
    {
        UImodule::on_window_update_begin();

        bool show = true;
        UImodule::show_example_app_dock_space(&show);
        
        ImGui::Begin("Info");
        ImGui::Text(("Mouse position X: " +  std::to_string(m_mouse_pos_x)).c_str());
        ImGui::Text(("Mouse position Y: " + std::to_string(m_mouse_pos_y)).c_str());
        ImGui::End();
        
        ImGui::Begin("Aera settings");
        ImGui::ColorEdit4("Background Color", m_colors);
        ImGui::ColorPicker3("Light source Color", m_light_color);
        ImGui::SliderFloat3("Line final position", m_line_pos, -50.f, 50.f);
        ImGui::Checkbox("Draw null intersection", &m_drawNullIntersection);
        ImGui::End();
        int a = 0;
        ImGui::Begin("Something settings");
        ImGui::SliderFloat3("Sprite position", m_sprite_pos, -50.f, 50.f);
        ImGui::SliderFloat3("Cube position", m_cube_pos, -50.f, 50.f);
        ImGui::SliderFloat3("Cube scale", m_cube_scale, -50.f, 50.f);
        ImGui::SliderFloat3("Cube rotation", m_cube_rot, 0.f, 360.f);
        ImGui::SliderFloat3("Light source position", m_light_pos, -20.f, 20.f);
        ImGui::SliderFloat("Ambient factor", &m_ambient_factor, 0.f, 1.f);
        ImGui::SliderFloat("Diffuse factor", &m_diffuse_factor, 0.f, 1.f);
        ImGui::SliderFloat("Specular factor", &m_specular_factor, 0.f, 1.f);
        ImGui::SliderFloat("Shininess", &m_shininess, 0.f, 100.f);
        ImGui::SliderInt("Is metalic", &m_isMetalic, 0, 2);
        //ImGui::BeginListBox("ASS");
        //ImGui::ListBox("WE", &a, &b, 3);
        //ImGui::EndListBox();
        ImGui::End();

        ImGui::Begin("Camera settings");
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
        if (ImGui::SliderFloat("Camera far clip plane", &m_cam_far_plane, 1.f, 300.f))
        {
            m_cam->set_far_clip_plane(m_cam_far_plane);
        }
        ImGui::SliderFloat("Sensetivity", &m_cam_sensetivity, 0.001f, 1.f);
        ImGui::SliderFloat("Addition speed", &m_add_ctrl_speed, 1.f, 4.f);
        if (ImGui::Checkbox("Perspective camera", &m_isPerspectiveCam))
        {
            m_cam->set_projection_mode(m_isPerspectiveCam ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
        }
        ImGui::Checkbox("Inversive mouse", &m_isInversiveMouseY);
        if (ImGui::Button("Reset position"))
        {
            m_cam_pos[0] = 0;
            m_cam_pos[1] = 0;
            m_cam_pos[2] = 0;
            m_cam->set_position(glm::vec3(m_cam_pos[0], m_cam_pos[1], m_cam_pos[2]));
        }
        if (ImGui::Button("Reset rotation"))
        {
            m_cam_rot[0] = 0;
            m_cam_rot[1] = 0;
            m_cam_rot[2] = 0;
            m_cam->set_rotation(glm::vec3(m_cam_rot[0], m_cam_rot[1], m_cam_rot[2]));
        }
        ImGui::End();

        UImodule::on_window_update_draw();
    }
private:
};

int main(int argc, char** argv)
{
    glm::ivec2 g_WindowSize(800, 600);

    //IGame* g_Game;
    ResourceManager::setExecutablePath(argv[0]);
    //g_Game = new TanksDemoGame(g_WindowSize);
    //g_Game = new SnakeDemoGame(g_WindowSize, glm::vec2(100));
    //g_Game = new PongDemoGame(g_WindowSize);

    Application* myApp = new MyApp();

    myApp->start(g_WindowSize, "Test game");

    delete myApp;

    return 0;
}