#include "EngineCore/GUI/ScrollBox.h"

#include "EngineCore/GUI/Sprite.h"
#include "EngineCore/GUI/GUI_place.h"
#include "EngineCore/Renderer/Line.h"
#include "EngineCore/System/SysFunc.h"

#define DISPLACEMENT_ELEMENT_Y 25.f
#define SHIFT_ELEMENT_Y 30.f

namespace GUI
{
	ScrollBox::ScrollBox(Sprite* backgrond, glm::vec2 pos, glm::vec2 scale, std::string name, int max_count_elements,
		std::shared_ptr<RenderEngine::Material> pMaterialLine, bool has_shift, GUI_place* place, bool isHorisontal, bool right_line, float scrollMultiplier, float percentDisplaysment)
		: GUI_element(name)
		, m_max_count_elements(max_count_elements)
		, m_background(std::move(backgrond))
		, m_place(std::move(place))
		, m_has_shift(has_shift)
		, m_isHorisontal(isHorisontal)
		, m_perc_disp(percentDisplaysment)
		, m_onLine(pMaterialLine != nullptr)
		, m_count_elements(0)
		, m_cur_scroll(0)
		, m_disp_scroll(0)
		, m_right_line(right_line)
		, m_scroll_mult(scrollMultiplier)
	{
		m_position_p = pos;
		m_scale_p = scale;
		if (m_background != nullptr)
		{
			m_background->set_position_p(pos);
			m_background->set_scale_p(scale);
		}

		if (m_onLine)
		{
			m_line = new RenderEngine::Line(std::move(pMaterialLine), GUI_place::get_pix_percent(glm::vec2(1.f, 0.f)).x);
		}
	}
	ScrollBox::~ScrollBox()
	{
		for (size_t i = 0; i < m_elements.size(); i++)
		{
			delete m_elements.at(i);
		}
		m_elements.clear();
	}
	void ScrollBox::on_render_prj(glm::mat4& prj)
	{
		if (!m_isActive) return;
		for (size_t i = 0; i < m_elements.size(); i++)
		{
			GUI_element* cur = m_elements[i];
			if (m_isHorisontal)
			{
				if (cur->get_position().x < m_scale.x + m_position.x - (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().x)
					&& cur->get_position().x > m_position.x - m_scale.x + (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().x)) cur->set_active(true);
				else cur->set_active(false);
			}
			else
			{
				if (cur->get_position().y < m_scale.y + m_position.y - (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().y)
					&& cur->get_position().y > m_position.y - m_scale.y + (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().y)) cur->set_active(true);
				else cur->set_active(false);
			}
		}
		if (m_onLine)
		{
			m_line->set_prj_mat(prj);
			m_line->render(m_pos_line, glm::vec3(0.f, m_scale.y / (float)m_count_elements, m_layer + 1.f), glm::vec4(1.f));
		}
	}
	// need fix for add elements in render list in gui place
	std::vector<GUI_element*> ScrollBox::get_elements()
	{
		std::vector<GUI_element*> vec;
		if (m_background != nullptr)
		{
			add_tree_element(m_background);
			m_background->set_layer(m_layer);
			vec.push_back(m_background);
		}
		return vec;
	}
	// need fix overscrolling
	void ScrollBox::on_mouse_scroll(int offset)
	{
		if (!m_isActive) return;
		if (m_elements.size() < 2) return;
		if (m_isHorisontal)
		{
			if (((m_elements[0]->get_position().x < m_position.x + m_scale.x - m_elements[0]->get_scale().x - (m_has_shift ? SHIFT_ELEMENT_Y : 0)) && offset < 0) ||
				((m_elements.last()->get_position().x > m_position.x - m_scale.x + m_elements[0]->get_scale().x + (m_has_shift ? SHIFT_ELEMENT_Y : 0)) && offset > 0))
				return;
			for (size_t i = 0; i < m_elements.size(); i++)
			{
				GUI_element* cur = m_elements[i];
				cur->add_position(glm::vec2(offset * m_scroll_mult, 0.f));
				/*cur->add_position(glm::vec2(offset * m_scroll_mult, 0.f));
				if (cur->get_position().x < m_scale.x + m_position.x - (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().x)
					&& cur->get_position().x > m_position.x - m_scale.x + (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().x)) cur->set_active(true);
				else cur->set_active(false);*/
			}
		}
		else
		{
			if (((m_elements[0]->get_position().y < m_position.y + m_scale.y - m_elements[0]->get_scale().y - (m_has_shift ? SHIFT_ELEMENT_Y : 0)) && offset < 0) ||
				((m_elements.last()->get_position().y > m_position.y - m_scale.y + m_elements[0]->get_scale().y + (m_has_shift ? SHIFT_ELEMENT_Y : 0)) && offset > 0))
				return;

			m_cur_scroll += offset;
			if (m_cur_scroll < 0)
			{
				m_cur_scroll = 0;
			}
			else if(m_cur_scroll > m_disp_scroll)
			{
				m_cur_scroll = m_disp_scroll;
			}

			for (size_t i = 0; i < m_elements.size(); i++)
			{
				GUI_element* cur = m_elements[i];
				cur->add_position(glm::vec2(0.f, offset * m_scroll_mult));
				/*if (cur->get_position().y < m_scale.y + m_position.y - (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().y)
					&& cur->get_position().y > m_position.y - m_scale.y + (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().y)) cur->set_active(true);
				else cur->set_active(false);*/
			}

			m_pos_line.y = sysfunc::conv_range(m_disp_scroll - m_cur_scroll, 0, m_disp_scroll, 0, (2.f * m_scale.y) - (m_scale.y / (float)m_count_elements))
				+ m_position.y - m_scale.y;
		}
	}
	void ScrollBox::set_active(const bool state)
	{
		m_isActive = state;
		set_tree_active(state);
	}
	void ScrollBox::set_position(glm::vec2 pos)
	{ 
		m_position = pos; 
		set_tree_pos(pos); 
		if (m_elements[0] == nullptr) return;
		m_disp_scroll = (((m_position.y - m_scale.y) + m_elements[0]->get_scale().y + (m_has_shift ? SHIFT_ELEMENT_Y : 0)) - m_elements[0]->get_position().y) / m_scroll_mult;
		if (m_isHorisontal)
		{
			m_pos_line = glm::vec3(m_position.x + m_scale.x - GUI_place::get_pix_percent(glm::vec2(1.f, 0.f)).x,
				m_position.y + m_scale.y - (m_elements[0]->get_scale().y * m_perc_disp) / 2.f, m_layer + 2.f);
		}
		else
		{
			m_pos_line = glm::vec3(m_right_line ? (m_position.x + m_scale.x - GUI_place::get_pix_percent(glm::vec2(1.f, 0.f)).x) :
				(m_position.x - m_scale.x + GUI_place::get_pix_percent(glm::vec2(1.f, 0.f)).x),
				sysfunc::conv_range(m_disp_scroll, 0, m_disp_scroll, 0, (2.f * m_scale.y) - (m_scale.y / (float)m_count_elements)) + m_position.y - m_scale.y,
				m_layer + 2.f);
		}
	}
	void ScrollBox::add_element(GUI_element* element)
	{
		if (m_place != nullptr)
		{
			m_place->add_element(element);
		}
		if (m_has_shift)
		{
			element->set_position(glm::vec2(m_position.x - m_scale.x, m_position.y - m_scale.y + SHIFT_ELEMENT_Y)); // displacement X here
			element->set_scale(m_scale);
		}
		else
		{
			m_elements.push_back(element);
		}
		for (size_t i = 0; i < m_elements.size(); i++)
		{
			if (m_isHorisontal)
			{
				m_elements[i]->set_position(
					glm::vec2((m_position.x + m_scale.x - element->get_scale().x) - element->get_scale().y * m_perc_disp -
						(i * (2.f * element->get_scale().x + element->get_scale().y * m_perc_disp)), m_position.y));
			}
			else
			{
				if (m_has_shift)
					m_elements[(i - m_elements.size() + 1) * -1]->set_position(
						glm::vec2(m_position.x - m_scale.x, (m_position.y - m_scale.y + SHIFT_ELEMENT_Y) + ((i + 1) * SHIFT_ELEMENT_Y)));
				else
					m_elements[i]->set_position(
						glm::vec2(m_position.x,
							(m_position.y + m_scale.y - element->get_scale().y - element->get_scale().y * m_perc_disp) -
							(i * (2.f * element->get_scale().y + element->get_scale().y * m_perc_disp))));
			}
		}
		m_count_elements++;
		add_tree_element(element);
		if (m_has_shift)
		{
			m_elements.push_back(element);
		}
		else
		{

		}
		if (m_elements[1] == nullptr) set_position(m_position);
		if (m_elements.size() > m_max_count_elements) m_elements.remove((size_t)0);
	}
	void ScrollBox::clear()
	{
		for (size_t i = 0; i < m_elements.size(); i++)
		{
			delete m_elements.at(i);
		}
		m_elements.clear();
	}
	void ScrollBox::set_open(bool isOpen)
	{
		set_tree_active(isOpen);
	}
}