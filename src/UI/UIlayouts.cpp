#include "EngineCore/UI/UIlayouts.h"

#include "EngineCore/Renderer/ShaderProgramLayout.h"
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
            m_on_chanege(m_color, m_is_active);
        }
        if (ImGui::Checkbox("Active highlight", &m_is_active))
        {
            m_on_chanege(m_color, m_is_active);
        }
    }
}

void UIlayoutHighlight::set_callback(std::function<void(const float*, bool)> on_chanege)
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

UIlayoutShaderProgram::UIlayoutShaderProgram()
{
}

void UIlayoutShaderProgram::on_draw_ui()
{
    if (m_shaderLayout == nullptr) return;
    ImGui::Text("ShaderSettings");
    std::vector<RenderEngine::ShaderProgramLayoutElement> elements = m_shaderLayout->getLayoutElements();
    for (const auto& curElements : elements)
    {
        if (curElements.type > 5) continue;
        switch (curElements.type)
        {
        case Float:
            if (ImGui::SliderFloat(curElements.name.c_str(), get_data(curElements.name), curElements.min, curElements.max))
                m_on_chanege(curElements.name, get_data(curElements.name), Float);
            break;
        case Vec3:
            if (ImGui::SliderFloat3(curElements.name.c_str(), get_data(curElements.name), curElements.min, curElements.max))
                m_on_chanege(curElements.name, get_data(curElements.name), Vec3);
            break;
        case Vec4:
            if (ImGui::SliderFloat4(curElements.name.c_str(), get_data(curElements.name), curElements.min, curElements.max))
                m_on_chanege(curElements.name, get_data(curElements.name), Vec4);
            break;
        case Col3:
            if (ImGui::ColorEdit3(curElements.name.c_str(), get_data(curElements.name)))
                m_on_chanege(curElements.name, get_data(curElements.name), Col3);
            break;
        case Col4:
            if (ImGui::ColorEdit4(curElements.name.c_str(), get_data(curElements.name)))
                m_on_chanege(curElements.name, get_data(curElements.name), Col4);
            break;
        }
    }
}

void UIlayoutShaderProgram::set_shader_layout(std::shared_ptr<RenderEngine::ShaderProgramLayout> shaderLayout)
{
    if (!m_data_map.empty())
    {
        for (auto cuElement : m_data_map)
        {
            delete[] cuElement.second;
        }
        m_data_map.clear();
    }
    m_shaderLayout = std::move(shaderLayout);
    std::vector<RenderEngine::ShaderProgramLayoutElement> elements = m_shaderLayout->getLayoutElements();
    for (const auto& curElements : elements)
    {
        if (curElements.type > 5) continue;
        switch (curElements.type)
        {
        case Float:
            m_data_map.emplace(curElements.name, new float[1] {0});
            break;
        case Vec3:
            m_data_map.emplace(curElements.name, new float[3] {0, 0, 0});
            break;
        case Vec4:
            m_data_map.emplace(curElements.name, new float[4] {0, 0, 0, 0});
            break;
        case Col3:
            m_data_map.emplace(curElements.name, new float[3] {0, 0, 0});
            break;
        case Col4:
            m_data_map.emplace(curElements.name, new float[4] {0, 0, 0, 0});
            break;
        }
    }
}

void UIlayoutShaderProgram::set_callback(std::function<void(std::string nameProp, const void* data, EUITypeData type)> on_chanege)
{
    m_on_chanege = on_chanege;
}

float* UIlayoutShaderProgram::get_data(std::string name)
{
    std::map<std::string, float*>::const_iterator it = m_data_map.find(name);
    if (it != m_data_map.end())
    {
        return it->second;
    }
    return nullptr;
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
}
