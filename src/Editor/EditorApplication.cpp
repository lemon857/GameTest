#include "EngineCore/Editor/EditorApplication.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <chrono>
#include <array>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

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

const char* components[] = { "transform", "spriteRenderer", "meshRenderer", "highlight" };
const char* objstypes[] = { "empty object", "cube", "sprite", "objmodel", "plane" };

EditorApplication::EditorApplication()
    : Application()
{
    materialLayout = std::make_shared<UIlayoutMaterial>();
    meshrendererLayout.set_prop(materialLayout);

    transformLayout.set_callback(
        [&](const float* data, UIlayoutTransform::ETypeChanegedProp prop)
        {
            switch (prop)
            {
            case UIlayoutTransform::position:
                m_scene.get_list()[item_current]->object->getComponent<Transform>()->set_position(glm::vec3(data[0], data[1], data[2]));
                break;
            case UIlayoutTransform::scale:
                m_scene.get_list()[item_current]->object->getComponent<Transform>()->set_scale(glm::vec3(data[0], data[1], data[2]));
                break;
            case UIlayoutTransform::rotation:
                m_scene.get_list()[item_current]->object->getComponent<Transform>()->set_rotation(glm::vec3(data[0], data[1], data[2]));
                break;
            }
        });
    highlightLayout.set_callback(
        [&](const float* data, bool isActive, bool mode)
        {
            auto highlight = m_scene.get_list()[item_current]->object->getComponent<Highlight>();
            highlight->set_active(isActive);
            highlight->set_mode(mode);
            highlight->set_color(glm::vec3(data[0], data[1], data[2]));
        });
    materialLayout->set_callback(
        [&](const std::string textureName, const std::string shaderName)
        {
            std::shared_ptr<RenderEngine::Material> mat;
            if (m_scene.get_list()[item_current]->object->getComponent<MeshRenderer>() != nullptr)
            {
                MeshRenderer* mesh = m_scene.get_list()[item_current]->object->getComponent<MeshRenderer>();
                mat = mesh->get_material_ptr();
            }
            else if (m_scene.get_list()[item_current]->object->getComponent<SpriteRenderer>() != nullptr)
            {
                SpriteRenderer* sprite = m_scene.get_list()[item_current]->object->getComponent<SpriteRenderer>();
                mat = sprite->get_material_ptr();
            }
            else return;

            mat->set_shader(ResourceManager::getShaderProgram(shaderName));
            mat->set_texture(ResourceManager::getTexture(textureName));
        });
}
EditorApplication::~EditorApplication()
{
    delete m_cam;
    delete m_ray;
}
void EditorApplication::on_ui_render()
{
    UImodule::on_window_update_begin();
    
    bool show = true;
    UImodule::show_example_app_dock_space(&show);

    ImGui::ShowDemoWindow();

    ImGui::Begin("Info");
    ImGui::Text(("Mouse position X: " + std::to_string(m_mouse_pos_x)).c_str());
    ImGui::Text(("Mouse position Y: " + std::to_string(m_mouse_pos_y)).c_str());
    ImGui::Text("");
    ImGui::Text(("Mouse world direction X: " + std::to_string(m_world_mouse_dir_x)).c_str());
    ImGui::Text(("Mouse world direction Y: " + std::to_string(m_world_mouse_dir_y)).c_str());
    ImGui::Text(("Mouse world direction Z: " + std::to_string(m_world_mouse_dir_z)).c_str());
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
    if (ImGui::Button("Load new object file"))
        ImGui::OpenPopup("Load new file");
    if (ImGui::BeginPopupModal("Load new file"))
    {
        std::vector<std::string> files = ResourceManager::getNamesFilesInDirectory("res/models/");
        ImGui::Checkbox("Is reload", &isReloadObjFile);
        if (ImGui::BeginListBox("Models folder"))
        {
            for (int i = 0; i < files.size(); i++)
            {
                if (ImGui::Selectable(files[i].c_str(), i == file_current))
                {
                    file_current = i;
                }
            }
            ImGui::EndListBox();
        }
        if (ImGui::Button("Load model"))
        {
            ResourceManager::load_OBJ_file(files[file_current], isReloadObjFile);
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
    if (ImGui::Button("Save this scene"))
    {
        //ResourceManager::save_scene("res/scenes/my_scene.scene", m_scene);
    } 
    if (ImGui::Button("Load this scene"))
    {
        //ResourceManager::load_scene("res/scenes/my_scene.scene", m_scene);
    }
    ImGui::End();

    ImGui::Begin("Object manager");
    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered()) item_current = -1;
    ImGui::Text("Count: %i", m_scene.get_list().size());
    if (ImGui::Button("Add object"))
        ImGui::OpenPopup("Add object window");

    if (ImGui::BeginPopupModal("Add object window"))
    {
        std::vector<std::string> matNames = ResourceManager::getNamesMaterials();
        ImGui::Combo("Types", &objstype_current, objstypes, IM_ARRAYSIZE(objstypes));
        ImGui::Text("Materials");
        ImGui::Separator();
        if (ImGui::BeginListBox("Materials"))
        {
            for (int i = 0; i < matNames.size(); i++)
            {
                char buf[32];
                sprintf(buf, matNames[i].c_str());
                if (ImGui::Selectable(buf, material_current == i))
                {
                    material_current = i;
                }
            }
            ImGui::EndListBox();
        }
        if (ImGui::Button("Create"))
        {
            switch (objstype_current)
            {
            case 0:
                m_scene.add_object<EmptyObject>();
                ImGui::CloseCurrentPopup();
                break;
            case 1:
                m_scene.add_object<Cube>(ResourceManager::getMaterial(matNames[material_current]));
                ImGui::CloseCurrentPopup();
                break;
            case 2:
                //m_scene.add_object<Sprite>(ResourceManager::getMaterial(matNames[material_current]), "default");
                ImGui::CloseCurrentPopup();
                break;
            case 3:
                if (!isSelectedObjFile)
                {
                    ImGui::OpenPopup("Add object window -- ObjModel settings");
                }
                else
                {
                    m_scene.add_object<ObjModel>(selectedObj, ResourceManager::getMaterial(matNames[material_current]));
                    ImGui::CloseCurrentPopup();
                    isSelectedObjFile = false;
                }
                break;
            case 4:
                m_scene.add_object<Plane>(ResourceManager::getMaterial(matNames[material_current]));
                ImGui::CloseCurrentPopup();
                break;
            }
        }

        bool unused_open = true;
        if (ImGui::BeginPopupModal("Add object window -- ObjModel settings", &unused_open))
        {
            std::vector<std::string> objs = ResourceManager::getNamesObjs();

            if (ImGui::BeginListBox("Loaded objects"))
            {
                for (int i = 0; i < objs.size(); i++)
                {
                    if (ImGui::Selectable(objs[i].c_str(), i == obj_current))
                    {
                        obj_current = i;
                    }
                }
                ImGui::EndListBox();
            }
            if (ImGui::Button("Select ObjModel"))
            {
                selectedObj = objs[obj_current];
                isSelectedObjFile = true;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }

    for (int n = 0; n < m_scene.get_list().size(); n++)
    {
        char buf[32];
        sprintf(buf, m_scene.get_list()[n]->object->get_name().c_str());
        ImGui::Selectable(buf, item_current == n);
        if (ImGui::IsItemClicked())
        {
            sprintf(bufName, m_scene.get_list()[n]->object->get_name().c_str());
            item_current = n;
            if (m_scene.at(item_current)->getComponent<Transform>() != nullptr)
            {
                glm::vec3 pos = m_scene.at(item_current)->getComponent<Transform>()->get_position();
                glm::vec3 scale = m_scene.at(item_current)->getComponent<Transform>()->get_scale();
                glm::vec3 rot = m_scene.at(item_current)->getComponent<Transform>()->get_rotation();

                transformLayout.set_props(pos, scale, rot);
            }
            if (m_scene.at(item_current)->getComponent<Highlight>() != nullptr)
            {
                highlightLayout.set_color(m_scene.at(item_current)->getComponent<Highlight>()->get_color());
            }
            if (m_scene.at(item_current)->getComponent<MeshRenderer>() != nullptr)
            {
                materialLayout->set_material(m_scene.at(item_current)->getComponent<MeshRenderer>()->get_material_ptr());
            }
            if (m_scene.at(item_current)->getComponent<SpriteRenderer>() != nullptr)
            {
                //shaderLayout->set_shader_layout(m_objs[item_current]->getComponent<SpriteRenderer>()->get_material_ptr()->get_shader_ptr()->get_layout());
                //materialLayout->set_material(m_objs[item_current]->getComponent<SpriteRenderer>()->get_material_ptr());
            }
        }
        if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
        {
            item_current = n;
            ImGui::Text("This a prop for \"%s\"!", m_scene.get_list()[n]->object->get_name().c_str());
            ImGui::Combo("components", &component_current, components, IM_ARRAYSIZE(components));
            if (ImGui::Button("Add component"))
            {
                switch (component_current)
                {
                case 0:
                    if (m_scene.at(item_current)->addComponent<Transform>() != nullptr)  transformLayout.set_props(glm::vec3(0.f), glm::vec3(1.f), glm::vec3(0.f));
                    break;
                case 1:
                    if (m_scene.at(item_current)->addComponent<SpriteRenderer>(ResourceManager::getMaterial("default"), "default") == nullptr)
                        break;
                case 2:
                    if (!isSelectedObjFile)
                        ImGui::OpenPopup("Add component window -- MeshRenderer settings");
                    else {
                        if (m_scene.at(item_current)->addComponent<MeshRenderer>(ResourceManager::load_OBJ_file(selectedObj), ResourceManager::getMaterial("default")) == nullptr)
                        {
                            materialLayout->set_material(m_scene.at(item_current)->getComponent<MeshRenderer>()->get_material_ptr());
                        }
                    }
                    break;
                case 3:
                    if (m_scene.at(item_current)->addComponent<Highlight>(ResourceManager::getMaterial("default")) == nullptr)
                    {

                    }
                    break;
                }
            }
            if (ImGui::Button("Delete component"))
            {
                switch (component_current)
                {
                case 0:
                    if (m_scene.get_list()[item_current]->object->getComponent<Transform>() != nullptr)
                        m_scene.get_list()[item_current]->object->deleteComponent<Transform>();
                    break;
                case 1:
                    if (m_scene.get_list()[item_current]->object->getComponent<SpriteRenderer>() != nullptr)
                        m_scene.get_list()[item_current]->object->deleteComponent<SpriteRenderer>();
                    break;
                case 2:
                    if (m_scene.get_list()[item_current]->object->getComponent<MeshRenderer>() != nullptr)
                        m_scene.get_list()[item_current]->object->deleteComponent<MeshRenderer>();
                    break;
                case 3:
                    if (m_scene.get_list()[item_current]->object->getComponent<Highlight>() != nullptr)
                        m_scene.get_list()[item_current]->object->deleteComponent<Highlight>();
                    break;
                }
            }
            if (ImGui::Button("Rename"))
            {
                ImGui::OpenPopup("Rename object");
            }
            if (ImGui::Button("Delete this object"))
            {
                m_scene.get_list().remove(m_scene.get_list()[item_current]);

                if (item_current < m_scene.get_list().size() - 1)
                {
                    item_current = m_scene.get_list().size() - 1;
                }
            }
            if (ImGui::Button("Close"))
                ImGui::CloseCurrentPopup();

            if (ImGui::BeginPopupModal("Add component window -- MeshRenderer settings"))
            {
                std::vector<std::string> objs = ResourceManager::getNamesObjs();

                if (ImGui::BeginListBox("Loaded objects"))
                {
                    for (int i = 0; i < objs.size(); i++)
                    {
                        if (ImGui::Selectable(objs[i].c_str(), i == obj_current))
                        {
                            obj_current = i;
                        }
                    }
                    ImGui::EndListBox();
                }
                if (ImGui::Button("Select ObjModel"))
                {
                    selectedObj = objs[obj_current];
                    isSelectedObjFile = true;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
            if (ImGui::BeginPopupModal("Rename object"))
            {
                ImGui::InputText("Name", bufName, 32);
                if (ImGui::Button("Submit"))
                {
                    m_scene.get_list()[item_current]->object->set_name(std::string(bufName));
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            ImGui::EndPopup();
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Right-click to open prop");
    }
    ImGui::End();

    ImGui::Begin("Object settings"); 
    if (item_current != -1)
    {
        auto obj = m_scene.at(item_current);
        if (obj->getComponent<Transform>() != nullptr && item_current >= 0)
        {
            transformLayout.set_props(
                obj->getComponent<Transform>()->get_position(),
                obj->getComponent<Transform>()->get_scale(),
                obj->getComponent<Transform>()->get_rotation());
            transformLayout.on_draw_ui();
        }
        if (obj->getComponent<Highlight>() != nullptr && item_current >= 0)
        {
            highlightLayout.on_draw_ui();
        }
        if (obj->getComponent<MeshRenderer>() != nullptr && item_current >= 0)
        {
            meshrendererLayout.on_draw_ui();
        }
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
    ImGui::SliderFloat("Distance", &m_distance, 1.f, 50.f);
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

    m_isUIhovered = ImGui::IsAnyItemHovered() || ImGui::IsAnyItemFocused() || ImGui::IsAnyItemActive();
}
bool EditorApplication::init()
{
    m_cam = new Camera(glm::vec3(0), glm::vec3(0));

    m_cam->set_viewport_size(static_cast<float>(m_pWindow->get_size().x), static_cast<float>(m_pWindow->get_size().y));

    std::vector<std::string> names;

    names.push_back("default3DShader");

    //m_scene.add_object<ObjModel>("res/models/monkey.obj", ResourceManager::getMaterial("monkey"));
    m_scene.add_object<DirectionalLight>(names);
    m_scene.add_object<PointerLight>(names, 1.1f);
    //add_object<Cube>(ResourceManager::getMaterial("cube"));
    //add_object<Cube>(ResourceManager::getMaterial("default")); 
    //add_object<Sprite>(ResourceManager::getMaterial("cube"), "YellowUp11");

    ((float*)ResourceManager::getMaterial("monkey")->get_data("ambient_factor"))[0] = 0.33f;
    ((float*)ResourceManager::getMaterial("cube")->get_data("ambient_factor"))[0] = 0.33f;

    m_line = new RenderEngine::Line(ResourceManager::getMaterial("default"));
    m_line_transform = new RenderEngine::Line(ResourceManager::getMaterial("default"), 10);

    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.33f, 0.33f, 0.33f, 1.f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.11f, 0.11f, 0.11f, 1.f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.44f, 0.44f, 1.f);

    style.Colors[ImGuiCol_Button] = ImVec4(0.4f, 0.4f, 0.4f, 1.f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.f);

    style.Colors[ImGuiCol_Tab] = ImVec4(0.5f, 0.5f, 0.5f, 1.f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.f);

    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.282f, 0.282f, 0.282f, 1.f);

    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.228f, 0.228f, 0.228f, 1.f);

    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);

    style.Colors[ImGuiCol_Header] = ImVec4(0.38f, 0.38f, 0.38f, 1.f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.5f, 0.5f, 0.5f, 1.f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.4f, 0.4f, 1.f);

    return true;
}
void EditorApplication::on_key_update(const double delta)
{
    if (m_isUIhovered) return;
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
        rotation_delta.x += static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_DOWN))
    {
        rotation_delta.x -= static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
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
        rotation_delta.z += static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
    }
    else if (Input::isKeyPressed(KeyCode::KEY_E))
    {
        rotation_delta.z -= static_cast<float>(addSpeed * m_cam_rotate_velocity * delta);
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
void EditorApplication::on_update(const double delta)
{
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

    ResourceManager::getShaderProgram("colorShader")->use();
    ResourceManager::getShaderProgram("colorShader")->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_cam->get_projection_matrix() * m_cam->get_view_matrix());
    //ResourceManager::getShaderProgram("colorShader")->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_cam->get_ui_matrix());
   
    ResourceManager::getShaderProgram("default3DShader")->use();
    ResourceManager::getShaderProgram("default3DShader")->setVec3("cam_position", m_cam->get_position());
    ResourceManager::getShaderProgram("default3DShader")->setFloat("ambient_factor", 0.33f);

    //m_line->render_from_to(glm::vec3(0.f, 0.f, 0.f), glm::vec3(m_mouse_pos_x, (float)m_pWindow->get_size().y - m_mouse_pos_y, 0.f), glm::vec3(1.f, 1.f, 1.f));

    if (m_drawNullIntersection)
    {
        m_line->render_from_to(glm::vec3(0.f, 0.f, 50.f), glm::vec3(0.f, 0.f, -50.f), glm::vec3(0.f, 0.f, 1.f));
        m_line->render_from_to(glm::vec3(0.f, 50.f, 0.f), glm::vec3(0.f, -50.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        m_line->render_from_to(glm::vec3(50.f, 0.f, 0.f), glm::vec3(-50.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
    }

    if (item_current != -1)
    {
        if (m_scene.at(item_current)->getComponent<Transform>() != nullptr)
    {

        if (m_moveObject)
        {
            glm::mat4 translateMat(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                pos[0], pos[1], pos[2], 1);

            glm::vec3 posfin = glm::vec3(translateMat * glm::vec4(glm::vec3(m_world_mouse_dir_x, m_world_mouse_dir_y, m_world_mouse_dir_z) * m_distance, 1.f));
            m_scene.at(item_current)->getComponent<Transform>()->set_position(posfin);
        }
        m_line->render(m_scene.at(item_current)->getComponent<Transform>()->get_position(), glm::vec3(5.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
        m_line->render(m_scene.at(item_current)->getComponent<Transform>()->get_position(), glm::vec3(0.f, 0.f, 5.f), glm::vec3(0.f, 0.f, 1.f));
        m_line->render(m_scene.at(item_current)->getComponent<Transform>()->get_position(), glm::vec3(0.f, 5.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    }
    }

    for (size_t i = 0; i < m_scene.get_list().size(); i++)
    {
        MeshRenderer* mesh = m_scene.get_list()[i]->object->getComponent<MeshRenderer>();
        if (mesh != nullptr)
        {
            std::shared_ptr<RenderEngine::ShaderProgram> shader = mesh->get_material_ptr()->get_shader_ptr();
            shader->use();
            shader->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_cam->get_projection_matrix() * m_cam->get_view_matrix());
        }
        m_scene.at(i)->update(delta);
    }

    fps++;
    curTime += delta;

    if (curTime >= 1000)
    {
        fpses[curFpsEs] = fps;
        curFpsEs++;
        if (curFpsEs >= countFpsEs)
        {
            int sum = 0;
            for (int cur : fpses)
            {
                sum += cur;
            }
            m_fps = (int)((float)sum / (float)countFpsEs);
            curFpsEs = 0;
        }
        curTime = 0;
        fps = 0;
    }
}
bool EditorApplication::init_events()
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

            glm::vec3 pos;

            pos.x = objcoord.x - m_cam->get_position().x;
            pos.y = objcoord.y - m_cam->get_position().y;
            pos.z = objcoord.z - m_cam->get_position().z;

            pos = glm::normalize(pos);

            m_world_mouse_dir_x = pos.x;
            m_world_mouse_dir_y = pos.y;
            m_world_mouse_dir_z = pos.z;

            m_mouse_pos_x = e.x;
            m_mouse_pos_y = e.y;
            LOG_INFO("[EVENT] Mouse moved to {0}x{1}", e.x, e.y);
        });
    m_event_dispather.add_event_listener<EventMouseScrolled>(
        [&](EventMouseScrolled& e)
        {
            if (m_distance + e.y_offset > 0) m_distance += e.y_offset;
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