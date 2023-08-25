#include "EngineCore/UI/UIlayouts.h"

void UIlayoutTransform::on_draw_ui()
{
    if (ImGui::TreeNode("Transform"))
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
        ImGui::TreePop();
    }
}

void UIlayoutTransform::set_on_chanege_callback(std::function<void(float*, ETypeChanegedProp)> on_chanege)
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
    if (ImGui::TreeNode("Highlight"))
    {
        if (ImGui::ColorEdit3("Color", m_color))
        {
            m_on_chanege(m_color, m_is_active);
        }
        if (ImGui::Checkbox("Active highlight", &m_is_active))
        {
            m_on_chanege(m_color, m_is_active);
        }
        ImGui::TreePop();
    }
}

void UIlayoutHighlight::set_on_chanege_callback(std::function<void(float*, bool)> on_chanege)
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