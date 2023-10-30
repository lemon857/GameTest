#include "EngineCore/GUI/GUI_place.h"
#include "EngineCore/GUI/GUI_element.h"

#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Renderer/ShaderProgram.h"

#include "EngineCore/System/ShadersSettings.h"
#include "EngineCore/Renderer/Camera.h"

#include "EngineCore/System/Log.h"

namespace GUI
{
	GUI_place::GUI_place(Camera* render_cam, std::shared_ptr<RenderEngine::Material> pMaterial)
		: m_pMaterial(std::move(pMaterial))
		, m_render_cam(std::move(render_cam))
	{
	}

	void GUI_place::on_update(const double delta)
	{
		for (auto cur : m_elements)
		{
			cur->on_update(delta);
		}
	}
	void GUI_place::on_render()
	{
		m_pMaterial->use();
		m_pMaterial->get_shader_ptr()->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_render_cam->get_ui_matrix());
		//glm::vec2 size = m_render_cam->get_viewport_size();
		for (auto cur : m_elements)
		{
			cur->on_render();
			cur->on_render_prj(m_render_cam->get_ui_matrix());
		}
	}
	void GUI_place::add_element(GUI_element* element)
	{
		m_elements.push_back(element);
	}
	GUI_element* GUI_place::get_element(size_t index)
	{
		return m_elements[index];
	}
	void GUI_place::on_mouse_release(int x, int y)
	{
		m_isFocus = false;
	}
	bool GUI_place::get_focus()
	{
		return m_isFocus;
	}
	void GUI_place::on_mouse_press(int x, int y)
	{
		glm::vec2 VPsize = m_render_cam->get_viewport_size();
		y = VPsize.y - y; // set null pos in left down
		for (auto cur : m_elements)
		{
			if (!cur->get_active()) continue;
			glm::vec2 scale = cur->get_scale();
			glm::vec2 pos = cur->get_position() - scale;
			scale *= 2;
			if ((x >= pos.x && y >= pos.y && x <= pos.x + scale.x && y <= pos.y + scale.y))
			{
				cur->on_click();
				m_isFocus = true;
				LOG_INFO("[GUI] Click on object: {0}", cur->get_name());
			}
		}
	}
}