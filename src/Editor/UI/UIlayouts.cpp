#include "EngineCore/Editor/UI/UIlayouts.h"

#include "EngineCore/Renderer/ShaderProgramLayout.h"
#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/Material.h"

#include "EngineCore/Resources/ResourceManager.h"

void UIlayoutTransform::on_draw_ui()
{
    if (ImGui::CollapsingHeader("Transform"))
    {
        if (ImGui::DragFloat3("position", m_prop_pos, 0.1f, 1.f))
            m_on_chanege(m_prop_pos, position);
        if (ImGui::DragFloat3("scale", m_prop_scale, 0.1f, 1.f))
            m_on_chanege(m_prop_scale, scale);
        if (ImGui::DragFloat3("rotation", m_prop_rot, 0.1f, 1.f))
            m_on_chanege(m_prop_rot, rotation);
        if (ImGui::Button("Reset position"))
        {
            m_prop_pos[0] = 0.f;
            m_prop_pos[1] = 0.f;
            m_prop_pos[2] = 0.f;
            m_on_chanege(m_prop_pos, position);
        }
        if (ImGui::Button("Reset scale"))
        {
            m_prop_scale[0] = 1.f;
            m_prop_scale[1] = 1.f;
            m_prop_scale[2] = 1.f;
            m_on_chanege(m_prop_scale, scale);
        }
        if (ImGui::Button("Reset rotation"))
        {
            m_prop_rot[0] = 0.f;
            m_prop_rot[1] = 0.f;
            m_prop_rot[2] = 0.f;
            m_on_chanege(m_prop_rot, rotation);
        }
    }
}

void UIlayoutTransform::set_callback(std::function<void(const float*, ETypeChanegedProp)> on_chanege)
{
    m_on_chanege = on_chanege;
}

void UIlayoutTransform::set_props(glm::vec3 pos, glm::vec3 scale, glm::vec3 rot)
{
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

void UIlayoutHighlight::on_draw_ui()
{
    if (ImGui::CollapsingHeader("Highlight"))
    {
        if (ImGui::ColorEdit3("Color", m_color))
        {
            m_on_chanege(m_color, m_is_active, m_mode);
        }
        if (ImGui::Checkbox("Active highlight", &m_is_active))
        {
            m_on_chanege(m_color, m_is_active, m_mode);
        }
        if (ImGui::Checkbox("Mode", &m_mode))
        {
            m_on_chanege(m_color, m_is_active, m_mode);
        }
    }
}

void UIlayoutHighlight::set_callback(std::function<void(const float*, bool, bool)> on_chanege)
{
    m_on_chanege = on_chanege;
}

void UIlayoutHighlight::set_color(glm::vec3 color)
{
    m_is_active = true;
    m_color[0] = color.x;
    m_color[1] = color.y;
    m_color[2] = color.z;
}

UIlayoutMaterial::UIlayoutMaterial()
{
}

void UIlayoutMaterial::on_draw_ui()
{
    std::vector<std::string> shaderNames = ResourceManager::getNamesShadersProgram();
    std::vector<std::string> textureNames = ResourceManager::getNamesTextures2D();

    ImGui::Text("MaterialSettings");
    if (ImGui::Button("Switch shader"))
        ImGui::OpenPopup("shader_select_popup");
    if (ImGui::Button("Switch texture"))
        ImGui::OpenPopup("texture_select_popup");

    if (ImGui::BeginPopup("shader_select_popup"))
    {
        ImGui::Text("Shaders");
        ImGui::Separator();
        for (int i = 0; i < shaderNames.size(); i++)
        {
            char buf[32];
            sprintf(buf, shaderNames[i].c_str());
            if (ImGui::Selectable(buf, item_current_shader == i))
            {
                item_current_shader = i;
                m_nameShaderProgram = shaderNames[i];
                m_on_chanege(m_nameTexture, m_nameShaderProgram);
            }
        }
        ImGui::EndPopup();
    }
    if (ImGui::BeginPopup("texture_select_popup"))
    {
        ImGui::Text("Textures");
        ImGui::Separator();
        for (int i = 0; i < textureNames.size(); i++)
        {
            char buf[32];
            sprintf(buf, textureNames[i].c_str());
            if (ImGui::Selectable(buf, item_current_shader == i))
            {
                item_current_shader = i;
                m_nameTexture = textureNames[i];
                m_on_chanege(m_nameTexture, m_nameShaderProgram);
            }
        }
        ImGui::EndPopup();
    }    

    ImGui::Text("ShaderSettings");
    std::vector<RenderEngine::ShaderProgramLayoutElement> elements = ResourceManager::getShaderProgram(m_nameShaderProgram)->get_layout()->getLayoutElements();
    for (const auto& curElements : elements)
    {
        if (curElements.type > 5) continue;
        switch (curElements.type)
        {
        case Int:
            ImGui::SliderInt(curElements.name.c_str(), (int*)m_pMat->get_data(curElements.name), curElements.min, curElements.max);
            break;
        case Float:
            ImGui::SliderFloat(curElements.name.c_str(), (float*)m_pMat->get_data(curElements.name), curElements.min, curElements.max);
            break;
        case Vec3:
            ImGui::SliderFloat3(curElements.name.c_str(), (float*)m_pMat->get_data(curElements.name), curElements.min, curElements.max);
            break;
        case Vec4:
            ImGui::SliderFloat4(curElements.name.c_str(), (float*)m_pMat->get_data(curElements.name), curElements.min, curElements.max);
            break;
        case Col3:
            ImGui::ColorEdit3(curElements.name.c_str(), (float*)m_pMat->get_data(curElements.name));
            break;
        case Col4:
            ImGui::ColorEdit4(curElements.name.c_str(), (float*)m_pMat->get_data(curElements.name));
            break;
        }
    }
}

void UIlayoutMaterial::set_callback(std::function<void(const std::string nameTexture, const std::string nameShaderProgram)> on_chanege)
{
    m_on_chanege = on_chanege;
}

void UIlayoutMaterial::set_material(std::shared_ptr<RenderEngine::Material> pMaterial)
{
    m_nameShaderProgram = ResourceManager::getNameShaderProgram(pMaterial->get_shader_ptr());
    m_nameTexture = ResourceManager::getNameTexture2D(pMaterial->get_texture_ptr());

    std::vector<std::string> shaderNames = ResourceManager::getNamesShadersProgram();
    std::vector<std::string> textureNames = ResourceManager::getNamesTextures2D();

    for (size_t i = 0; i < shaderNames.size(); i++)
    {
        if (shaderNames[i] == m_nameShaderProgram)
        {
            item_current_shader = i;
            break;
        }
    }
    for (size_t i = 0; i < textureNames.size(); i++)
    {
        if (textureNames[i] == m_nameTexture)
        {
            item_current_texture = i;
            break;
        }
    }
    m_pMat = std::move(pMaterial);
}

UIlayoutMeshRenderer::UIlayoutMeshRenderer()
    : m_current_material(0)
{
}

void UIlayoutMeshRenderer::on_draw_ui()
{
    if (ImGui::CollapsingHeader("Mesh renderer"))
    {
        if (ImGui::Button("Switch material"))
            ImGui::OpenPopup("material_select_popup");

        if (ImGui::BeginPopup("material_select_popup"))
        {
            std::vector<std::string> materialsNames = ResourceManager::getNamesMaterials();
            ImGui::Text("Materials");
            ImGui::Separator();
            for (int i = 0; i < materialsNames.size(); i++)
            {
                char buf[32];
                sprintf(buf, materialsNames[i].c_str());
                if (ImGui::Selectable(buf, m_current_material == i))
                {
                    m_current_material = i;
                    m_materialUI->set_material(ResourceManager::getMaterial(materialsNames[i]));
                }
            }
            ImGui::EndPopup();
        }
        m_materialUI->on_draw_ui();
    }
}

void UIlayoutMeshRenderer::set_callback(std::function<void(const std::string nameMaterial)> on_chanege)
{
    m_on_chanege = on_chanege;
}

void UIlayoutMeshRenderer::set_prop(std::shared_ptr<UIlayoutMaterial> materialUI)
{
    m_materialUI = std::move(materialUI);
}
