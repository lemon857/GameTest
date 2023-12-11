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
	GUI_place::GUI_place(Camera* render_cam, std::shared_ptr<RenderEngine::Material> pMaterial)
		: m_pMaterial(std::move(pMaterial))
		, m_render_cam(std::move(render_cam))
	{
	}

	void GUI_place::on_update(const double delta)
	{
		for (auto cur : m_elements)
		{
			cur.second->on_update(delta);
		}
	}
	void GUI_place::on_render()
	{
		if (!m_isActive) return;
		//m_pMaterial->use();
		//m_pMaterial->get_shader_ptr()->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_render_cam->get_ui_matrix());
		//glm::vec2 size = m_render_cam->get_viewport_size();
		for (auto cur : m_render_list)
		{
			cur->on_render_prj(m_render_cam->get_ui_matrix());
		}
	}
	void GUI_place::add_element(GUI_element* element)
	{
		glm::vec2 posp = element->get_position_p();
		glm::vec2 scalep = element->get_scale_p();
		glm::vec2 wsize = m_render_cam->get_viewport_size();
		element->set_position(glm::vec2(posp.x / 100 * wsize.x, posp.y / 100 * wsize.y));
		element->set_scale(glm::vec2(scalep.x / 100 * wsize.x, scalep.y / 100 * wsize.y));
		m_render_list.push_back(element);
		add_elements(element->get_elements(), wsize);
		m_elements.emplace(element->get_name(), element);
	}
		
	void GUI_place::on_mouse_release(int x, int y)
	{
		m_isFocus = false;
		for (auto cur : m_elements)
		{
			cur.second->on_release();
		}
	}
	void GUI_place::on_resize()
	{
		glm::vec2 wsize = m_render_cam->get_viewport_size();
		for (auto cur : m_render_list)
		{
			glm::vec2 posp = cur->get_position_p();
			glm::vec2 scalep = cur->get_scale_p();
			cur->set_position(glm::vec2(posp.x / 100 * wsize.x, posp.y / 100 * wsize.y));
			cur->set_scale(glm::vec2(scalep.x / 100 * wsize.x, scalep.y / 100 * wsize.y));
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
	void GUI_place::add_elements(std::vector<GUI_element*> elements, glm::vec2 wsize)
	{
		if (elements.empty()) return;
		for (auto cur : elements)
		{
			glm::vec2 posp = cur->get_position_p();
			glm::vec2 scalep = cur->get_scale_p();
			cur->set_position(glm::vec2(posp.x / 100 * wsize.x, posp.y / 100 * wsize.y));
			cur->set_scale(glm::vec2(scalep.x / 100 * wsize.x, scalep.y / 100 * wsize.y));
			m_render_list.push_back(cur); // need sort with layers
			add_elements(cur->get_elements(), wsize);
		}

	}
	void GUI_place::on_mouse_press(int x, int y)
	{
		if (!m_isActive) return;
		glm::vec2 VPsize = m_render_cam->get_viewport_size();
		y = VPsize.y - y; // set null pos in left down
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