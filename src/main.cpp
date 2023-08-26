#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <iostream>
#include <chrono>
#include <array>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "EngineCore/Window.h"
#include "EngineCore/UI/UImodule.h"
#include "EngineCore/UI/UIlayouts.h"

#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Physics/Collider.h"

#include "EngineCore/Physics/PhysicsEngine.h"
#include "EngineCore/Application.h"
#include "EngineCore/System/Log.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Components/Highlight.h"
#include "EngineCore/Components/SpriteRenderer.h"
#include "EngineCore/Components/MeshRenderer.h"

const char* components[] = { "transform", "spriteRenderer", "meshRenderer", "highlight" };
const char* items[] = { "transform", "spriteRenderer", "meshRenderer", "highlight" };

int item_current = 0;
int component_current = 0;

float test = 0;

UIlayoutTransform transformLayout;
UIlayoutHighlight highlightLayout;
UIlayoutShaderProgram shaderLayout;
UIlayoutMaterial materialLayout;

class EditorApplication : public Application
{
public:
    EditorApplication()
        : Application()
    {
        transformLayout.set_callback(
            [&](const float* data, UIlayoutTransform::ETypeChanegedProp prop)
            {
                switch (prop)
                {
                case UIlayoutTransform::position:
                    m_objs[item_current]->getComponent<Transform>()->set_position(glm::vec3(data[0], data[1], data[2]));
                    break;
                case UIlayoutTransform::scale:
                    m_objs[item_current]->getComponent<Transform>()->set_scale(glm::vec3(data[0], data[1], data[2]));
                    break;
                case UIlayoutTransform::rotation:
                    m_objs[item_current]->getComponent<Transform>()->set_rotation(glm::vec3(data[0], data[1], data[2]));
                    break;
                }
            });
        highlightLayout.set_callback(
            [&](const float* data, bool isActive)
            {
                m_objs[item_current]->getComponent<Highlight>()->set_active(isActive);
                m_objs[item_current]->getComponent<Highlight>()->set_color(glm::vec3(data[0], data[1], data[2]));
            });
        shaderLayout.set_callback(
            [&](std::string name, const void* data, UIlayoutShaderProgram::EUITypeData type)
            {
                MeshRenderer* mesh = m_objs[item_current]->getComponent<MeshRenderer>();
                std::shared_ptr<RenderEngine::ShaderProgram> shader = mesh->get_material_ptr()->get_shader_ptr();
                switch (type)
                {
                case UIlayoutShaderProgram::Float:
                    shader->use();
                    shader->setFloat(name, *(float*)data);
                    break;
                case UIlayoutShaderProgram::Vec3:
                    shader->use();
                    shader->setVec3(name, glm::vec3(((float*)data)[0], ((float*)data)[1], ((float*)data)[2]));
                    break;
                case UIlayoutShaderProgram::Vec4:
                    shader->use();
                    shader->setVec4(name, glm::vec4(((float*)data)[0], ((float*)data)[1], ((float*)data)[2], ((float*)data)[3]));
                    break;
                case UIlayoutShaderProgram::Col3:
                    shader->use();
                    shader->setVec3(name, glm::vec3(((float*)data)[0], ((float*)data)[1], ((float*)data)[2]));
                    break;
                case UIlayoutShaderProgram::Col4:
                    shader->use();
                    shader->setVec4(name, glm::vec4(((float*)data)[0], ((float*)data)[1], ((float*)data)[2], ((float*)data)[3]));
                    break;
                }
            });
        materialLayout.set_callback(
            [&](const std::string textureName, const std::string shaderName)
            {
                MeshRenderer* mesh = m_objs[item_current]->getComponent<MeshRenderer>();
                std::shared_ptr<RenderEngine::Material> mat = mesh->get_material_ptr();

                auto shader = ResourceManager::getShaderProgram(shaderName);
                mat->set_shader_and_texture(shader, ResourceManager::getTexture(textureName));
                shaderLayout.set_shader_layout(shader->get_layout());
            });
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
        ImGui::End();

        ImGui::Begin("Object manager");
        if (ImGui::Button("Add cube"))
        {
            add_object<Cube>(ResourceManager::getMaterial("default"), "default");
        }
        for (int n = 0; n < m_objs.size(); n++)
        {
            char buf[32];
            sprintf(buf, m_objs[n]->get_name().c_str());
            if (ImGui::Selectable(buf, item_current == n))
            {
                item_current = n;
                if (m_objs[item_current]->getComponent<Transform>() != nullptr)
                {
                    glm::vec3 pos = m_objs[item_current]->getComponent<Transform>()->get_position();
                    glm::vec3 scale = m_objs[item_current]->getComponent<Transform>()->get_scale();
                    glm::vec3 rot = m_objs[item_current]->getComponent<Transform>()->get_rotation();

                    transformLayout.set_props(pos, scale, rot);
                }
                if (m_objs[item_current]->getComponent<Highlight>() != nullptr)
                {
                    highlightLayout.set_color(m_objs[item_current]->getComponent<Highlight>()->get_color());
                }          
                if (m_objs[item_current]->getComponent<MeshRenderer>() != nullptr)
                {
                    //shaderLayout.set_shader_layout(m_objs[item_current]->getComponent<MeshRenderer>()->get_material_ptr()->get_shader_ptr()->get_layout());
                    //materialLayout.set_material(m_objs[item_current]->getComponent<MeshRenderer>()->get_material_ptr());
                }
            }
            if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
            {
                item_current = n;
                ImGui::Text("This a prop for \"%s\"!", m_objs[n]->get_name().c_str());
                ImGui::Combo("components", &component_current, components, IM_ARRAYSIZE(components));
                if (ImGui::Button("Add component"))
                {
                    switch (component_current)
                    {
                    case 0:
                        if (m_objs[item_current]->addComponent<Transform>() != nullptr)  transformLayout.set_props(glm::vec3(0.f), glm::vec3(1.f), glm::vec3(0.f));
                        break;
                    case 1:
                        //if (m_objs[item_current]->getComponent<SpriteRenderer>() == nullptr)
                        //    m_objs[item_current]->addComponent<SpriteRenderer>();
                        break;
                    case 2:
                        if (m_objs[item_current]->addComponent<MeshRenderer>(nullptr, ResourceManager::getMaterial("default")) == nullptr)
                        break;
                    case 3:
                        if (m_objs[item_current]->addComponent<Highlight>(ResourceManager::getMaterial("default")) == nullptr) highlightLayout.activate();
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
                if (ImGui::Button("Close"))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Right-click to open prop");
        }
        ImGui::End();

        ImGui::Begin("Object settings");        
        if (m_objs[item_current]->getComponent<Transform>() != nullptr)
        {
            transformLayout.set_props(
                m_objs[item_current]->getComponent<Transform>()->get_position(),
                m_objs[item_current]->getComponent<Transform>()->get_scale(), 
                m_objs[item_current]->getComponent<Transform>()->get_rotation());
            transformLayout.on_draw_ui();
        }
        if (m_objs[item_current]->getComponent<Highlight>() != nullptr)
        {
            highlightLayout.on_draw_ui();
        }
        if (m_objs[item_current]->getComponent<MeshRenderer>() != nullptr)
        {
            shaderLayout.on_draw_ui();
            materialLayout.on_draw_ui();
        }/*
        ImGui::SliderFloat("Ambient factor", &m_ambient_factor, 0.f, 1.f);
        ImGui::SliderFloat("Diffuse factor", &m_diffuse_factor, 0.f, 1.f);
        ImGui::SliderFloat("Specular factor", &m_specular_factor, 0.f, 1.f);
        if (ImGui::SliderFloat("Diffuse / Specular factor", &m_diffuse_specular_factor, 0.f, 1.f))
        {
            m_specular_factor = 1.f - m_diffuse_specular_factor;
            m_diffuse_factor = 1.f - m_specular_factor;
        }
        ImGui::SliderFloat("Shininess", &m_shininess, 0.f, 100.f);
        ImGui::SliderFloat("Metalic factor", &m_metalic_factor, 0.f, 1.f);*/
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
        ImGui::SliderFloat("Distance", &m_distance, 1.f, 50.f);
        ImGui::SliderFloat("Sensetivity", &m_cam_sensetivity, 0.001f, 1.f);
        ImGui::SliderFloat("Addition speed", &m_add_ctrl_speed, 1.f, 4.f);
        if (ImGui::Checkbox("Perspective camera", &m_isPerspectiveCam))
        {
            m_cam->set_projection_mode(m_isPerspectiveCam ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
            //shaderLayout.set_shader_layout(ResourceManager::getShaderProgram("shape3DShader")->get_layout());
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

    editorApplication->start(g_WindowSize, "Engine editor");

    delete editorApplication;

    return 0;
}