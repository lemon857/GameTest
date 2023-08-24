#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <iostream>
#include <chrono>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "EngineCore/Window.h"
#include "EngineCore/UI/UImodule.h"

#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Physics/Collider.h"

#include "EngineCore/Physics/PhysicsEngine.h"
#include "EngineCore/Application.h"
#include "EngineCore/System/Log.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Components/Highlight.h"
#include "EngineCore/Components/SpriteRenderer.h"
#include "EngineCore/Components/MeshRenderer.h"

const char* items[] = { "cube", "model", "model2", "model3", "model4", "lightSource", "sprite" };

const char* components[] = { "transform", "spriteRenderer", "meshRenderer", "highlight" };

int item_current = 0;
int component_current = 0;

class EditorApplication : public Application
{
public:
    EditorApplication()
        : Application()
    {

    }
    ~EditorApplication()
    {

    }
    void on_ui_render() override
    {
        UImodule::on_window_update_begin();

        bool show = true;
        UImodule::show_example_app_dock_space(&show);
        
        ImGui::ShowDemoWindow();        

        ImGui::Begin("Info");
        ImGui::Text(("Mouse position X: " +  std::to_string(m_mouse_pos_x)).c_str());
        ImGui::Text(("Mouse position Y: " + std::to_string(m_mouse_pos_y)).c_str());
        ImGui::Text("");
        ImGui::Text(("Mouse world position X: " + std::to_string(m_world_mouse_pos_x)).c_str());
        ImGui::Text(("Mouse world position Y: " + std::to_string(m_world_mouse_pos_y)).c_str());
        ImGui::Text(("Mouse world position Z: " + std::to_string(m_world_mouse_pos_z)).c_str());
        ImGui::Text("");
        ImGui::Text(("FPS: " + std::to_string(m_fps)).c_str());
        ImGui::End();
        
        ImGui::Begin("Aera settings");
        ImGui::ColorEdit4("Background Color", m_colors);
        ImGui::ColorEdit3("Light source Color", m_light_color);
        ImGui::Checkbox("Draw null intersection", &m_drawNullIntersection);
        if (ImGui::Combo("components", &component_current, components, IM_ARRAYSIZE(components)))
        {

        }
        if (ImGui::Button("Add component"))
        {
            switch (component_current)
            {
            case 0:
                if (m_objs[item_current]->getComponent<Transform>() == nullptr)
                    m_objs[item_current]->addComponent<Transform>();
                break;
            case 1:
                //if (m_objs[item_current]->getComponent<SpriteRenderer>() == nullptr)
                //    m_objs[item_current]->addComponent<SpriteRenderer>();
                break;
            case 2:
                //if (m_objs[item_current]->getComponent<MeshRenderer>() == nullptr)
                //    m_objs[item_current]->addComponent<MeshRenderer>();
                break;
            case 3:
                if (m_objs[item_current]->getComponent<Highlight>() == nullptr)
                    m_objs[item_current]->addComponent<Highlight>(ResourceManager::getMaterial("default"));
                break;
            }
        }
        if (ImGui::Button("Delete component"))
        {
            switch (component_current)
            {
            case 0:
                if (m_objs[item_current]->getComponent<Transform>() != nullptr)
                    m_objs[item_current]->deleteComponent<Transform>();
                break;
            case 1:
                if (m_objs[item_current]->getComponent<SpriteRenderer>() != nullptr)
                    m_objs[item_current]->deleteComponent<SpriteRenderer>();
                break;
            case 2:
                if (m_objs[item_current]->getComponent<MeshRenderer>() != nullptr)
                    m_objs[item_current]->deleteComponent<MeshRenderer>();
                break;
            case 3:
                if (m_objs[item_current]->getComponent<Highlight>() != nullptr)
                    m_objs[item_current]->deleteComponent<Highlight>();
                break;
            }
        }
        ImGui::End();

        ImGui::Begin("Something settings");
        if (ImGui::Combo("objs", &item_current, items, IM_ARRAYSIZE(items)))
        {
            if (m_objs[item_current]->getComponent<Transform>() != nullptr)
            {
                glm::vec3 pos = m_objs[item_current]->getComponent<Transform>()->get_position();
                glm::vec3 scale = m_objs[item_current]->getComponent<Transform>()->get_scale();
                glm::vec3 rot = m_objs[item_current]->getComponent<Transform>()->get_rotation();

                m_prop_pos[0] = pos.x;
                m_prop_pos[1] = pos.y;
                m_prop_pos[2] = pos.z;

                m_prop_scale[0] = scale.x;
                m_prop_scale[1] = scale.y;
                m_prop_scale[2] = scale.z;

                m_prop_rot[0] = rot.x;
                m_prop_rot[1] = rot.y;
                m_prop_rot[2] = rot.z;
            }
            if (m_objs[item_current]->getComponent<Highlight>() != nullptr)
            {
                m_isActiveHighlight = m_objs[item_current]->getComponent<Highlight>()->get_active();
            }
        }
        if (m_objs[item_current]->getComponent<Transform>() != nullptr)
        {
            if (ImGui::TreeNode("Transform"))
            {
                if (ImGui::DragFloat3("position", m_prop_pos, 0.1f, 1.f))
                {
                    m_objs[item_current]->getComponent<Transform>()->set_position(glm::vec3(m_prop_pos[0], m_prop_pos[1], m_prop_pos[2]));
                }
                if (ImGui::DragFloat3("scale", m_prop_scale, 0.1f, 1.f))
                {
                    m_objs[item_current]->getComponent<Transform>()->set_scale(glm::vec3(m_prop_scale[0], m_prop_scale[1], m_prop_scale[2]));
                }
                if (ImGui::DragFloat3("rotation", m_prop_rot, 0.1f, 1.f))
                {
                    m_objs[item_current]->getComponent<Transform>()->set_rotation(glm::vec3(m_prop_rot[0], m_prop_rot[1], m_prop_rot[2]));
                }
                if (ImGui::Button("Reset position"))
                {
                    m_objs[item_current]->getComponent<Transform>()->set_position(glm::vec3(0.f));
                    m_prop_pos[0] = 0.f;
                    m_prop_pos[1] = 0.f;
                    m_prop_pos[2] = 0.f;
                }
                if (ImGui::Button("Reset scale"))
                {
                    m_objs[item_current]->getComponent<Transform>()->set_scale(glm::vec3(1.f));
                    m_prop_scale[0] = 1.f;
                    m_prop_scale[1] = 1.f;
                    m_prop_scale[2] = 1.f;
                }
                if (ImGui::Button("Reset rotation"))
                {
                    m_objs[item_current]->getComponent<Transform>()->set_rotation(glm::vec3(0.f));
                    m_prop_rot[0] = 0.f;
                    m_prop_rot[1] = 0.f;
                    m_prop_rot[2] = 0.f;
                }
                ImGui::TreePop();
            }
        }
        if (m_objs[item_current]->getComponent<Highlight>() != nullptr)
        {
            if (ImGui::TreeNode("Highlight"))
            {
                if (ImGui::ColorPicker3("Color", m_prop_color))
                {
                    m_objs[item_current]->getComponent<Highlight>()->set_color(glm::vec3(m_prop_color[0], m_prop_color[1], m_prop_color[2]));
                }
                if (ImGui::Checkbox("Active highlight", &m_isActiveHighlight))
                {
                    m_objs[item_current]->getComponent<Highlight>()->set_active(m_isActiveHighlight);
                }
                ImGui::TreePop();
            }
        }
        ImGui::SliderFloat("Ambient factor", &m_ambient_factor, 0.f, 1.f);
        ImGui::SliderFloat("Diffuse factor", &m_diffuse_factor, 0.f, 1.f);
        ImGui::SliderFloat("Specular factor", &m_specular_factor, 0.f, 1.f);
        if (ImGui::SliderFloat("Diffuse / Specular factor", &m_diffuse_specular_factor, 0.f, 1.f))
        {
            m_specular_factor = 1.f - m_diffuse_specular_factor;
            m_diffuse_factor = 1.f - m_specular_factor;
        }
        ImGui::SliderFloat("Shininess", &m_shininess, 0.f, 100.f);
        ImGui::SliderFloat("Metalic factor", &m_metalic_factor, 0.f, 1.f); 
        if (ImGui::Button("Reload model"))
        {
            m_model->reload();
        }
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
        if (ImGui::SliderFloat("Camera sov", &m_cam_sov, 0.1f, 20.f))
        {
            m_cam->set_size_of_view(m_cam_sov);
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

    EditorApplication* editorApplication = new EditorApplication();

    editorApplication->start(g_WindowSize, "Test game");

    delete editorApplication;

    return 0;
}