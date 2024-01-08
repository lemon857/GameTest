#include "EngineCore/GUI/GUI_place.h"
#include "EngineCore/GUI/GUI_element.h"

#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Renderer/ShaderProgram.h"

#include "EngineCore/System/ShadersSettings.h"
#include "EngineCore/Renderer/Camera.h"

#include "EngineCore/System/Log.h"

#include "EngineCore/Resources/ResourceManager.h"

#include "EngineCore/Sound/Sound.h"

namespace GUI
{
	glm::vec2 GUI_place::m_vp_size;

	GUI_place::GUI_place(Camera* render_cam, std::shared_ptr<RenderEngine::Material> pMaterial)
		: m_pMaterial(std::move(pMaterial))
		, m_render_cam(std::move(render_cam))
	{
		m_vp_size = m_render_cam->get_viewport_size();
	}

	void GUI_place::on_update(const double delta)
	{
		for (auto cur : m_els)
		{
			cur->on_update(delta);
		}
	}
	void GUI_place::on_render()
	{
		if (!m_isActive) return;
		//m_pMaterial->use();
		//m_pMaterial->get_shader_ptr()->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_render_cam->get_ui_matrix());
		//glm::vec2 size = m_render_cam->get_viewport_size();
		for (auto cur : m_els)
		{
			cur->on_render_prj(m_render_cam->get_ui_matrix());
		}
	}
	void GUI_place::add_element(GUI_element* element)
	{
		glm::vec2 posp = element->get_position_p();
		glm::vec2 scalep = element->get_scale_p();
		m_vp_size = m_render_cam->get_viewport_size();
		element->set_position(glm::vec2(posp.x / 100 * m_vp_size.x, m_vp_size.y / 100 * m_vp_size.y));
		element->set_scale(glm::vec2(scalep.x / 100 * m_vp_size.x, m_vp_size.y / 100 * m_vp_size.y));
		m_els.push_back(element);
		add_elements(element->get_elements());
		m_elements.emplace(element->get_name(), element);
	}
		
	void GUI_place::on_mouse_release(int x, int y)
	{
		m_isFocus = false;
		for (auto cur : m_els)
		{
			cur->on_release();
		}
	}
	void GUI_place::on_resize()
	{
		m_vp_size = m_render_cam->get_viewport_size();
		for (auto cur : m_els)
		{
			glm::vec2 posp = cur->get_position_p();
			glm::vec2 scalep = cur->get_scale_p();
			cur->set_position(glm::vec2(posp.x / 100 * m_vp_size.x, posp.y / 100 * m_vp_size.y));
			cur->set_scale(glm::vec2(scalep.x / 100 * m_vp_size.x, scalep.y / 100 * m_vp_size.y));
		}
	}
	bool GUI_place::get_focus()
	{
		return m_isFocus;
	}
	void GUI_place::set_logging_active(bool active)
	{
		m_is_event_logging_active = active;
	}
	void GUI_place::set_active(bool active)
	{
		m_isActive = active;
	}
	glm::vec2 GUI_place::get_pix_percent(glm::vec2 percent)
	{
		return glm::vec2(percent.x / 100 * m_vp_size.x, percent.y / 100 * m_vp_size.y);
	}
	void GUI_place::add_elements(std::vector<GUI_element*> elements)
	{
		if (elements.empty()) return;
		for (auto cur : elements)
		{
			glm::vec2 posp = cur->get_position_p();
			glm::vec2 scalep = cur->get_scale_p();
			cur->set_position(glm::vec2(posp.x / 100 * m_vp_size.x, posp.y / 100 * m_vp_size.y));
			cur->set_scale(glm::vec2(scalep.x / 100 * m_vp_size.x, scalep.y / 100 * m_vp_size.y));
			m_elements.emplace(cur->get_name(), cur);
			m_els.push_back(cur);
			add_elements(cur->get_elements());
		}
	}
	void GUI_place::add_elements(std::vector<GUI_element*> elements, GUI_element* tree_parent)
	{
		if (elements.empty()) return;
		for (auto cur : elements)
		{
			glm::vec2 posp = cur->get_position_p();
			glm::vec2 scalep = cur->get_scale_p();
			cur->set_position(glm::vec2(posp.x / 100 * m_vp_size.x, posp.y / 100 * m_vp_size.y));
			cur->set_scale(glm::vec2(scalep.x / 100 * m_vp_size.x, scalep.y / 100 * m_vp_size.y));
			tree_parent->add_tree_element(cur);
			m_elements.emplace(cur->get_name(), cur);
			m_els.push_back(cur);
			add_elements(cur->get_elements());
		}
	}

	void GUI_place::on_mouse_press(int x, int y)
	{
		if (!m_isActive) return;
		m_vp_size = m_render_cam->get_viewport_size();
		y = m_vp_size.y - y; // set null pos in left down
		for (auto cur : m_elements)
		{
			if (!cur.second->get_active()) continue;
			glm::vec2 scale = cur.second->get_scale();
			glm::vec2 pos = cur.second->get_position() - scale;
			scale *= 2;
			if ((x >= pos.x && y >= pos.y && x <= pos.x + scale.x && y <= pos.y + scale.y))
			{
				cur.second->on_press();
				m_isFocus = true;
				ResourceManager::get_sound("click")->play();
				if (m_is_event_logging_active) LOG_INFO("[GUI] Click on object: {0}", cur.second->get_name());
			}
		}
	}
}