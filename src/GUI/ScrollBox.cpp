#include "EngineCore/GUI/ScrollBox.h"

#include "EngineCore/GUI/Sprite.h"

#define SCROLL_MYLTIPLIER 10

#define DISPLACEMENT_ELEMENT_Y 17.f

namespace GUI
{
	ScrollBox::ScrollBox(Sprite* backgrond, glm::vec2 pos, glm::vec2 scale, std::string name, int max_count_elements)
		: GUI_element(name)
		, m_max_count_elements(max_count_elements)
		, m_background(std::move(backgrond))
	{
		m_position_p = pos;
		m_scale_p = scale;
		m_background->set_position_p(pos);
		m_background->set_scale_p(scale);
	}
	ScrollBox::~ScrollBox()
	{
	}
	void ScrollBox::on_render_prj(glm::mat4& prj)
	{
		for (size_t i = 0; i < m_elements.size(); i++)
		{
			GUI_element* cur = m_elements[i];
			if (cur->get_position().y < m_scale.y + m_position.y - DISPLACEMENT_ELEMENT_Y && cur->get_position().y > m_position.y - m_scale.y + DISPLACEMENT_ELEMENT_Y)
			{
				cur->on_render_prj(prj);
			}
		}
		m_background->on_render_prj(prj);
	}
	void ScrollBox::set_position(glm::vec2 pos)
	{
		m_background->set_position(pos);
		m_position = pos;
	}
	void ScrollBox::set_scale(glm::vec2 scale)
	{
		m_background->set_scale(scale);
		m_scale = scale;
	}
	// need fix overscrolling
	void ScrollBox::on_scroll(int offset)
	{
		for (size_t i = 0; i < m_elements.size(); i++)
		{
			GUI_element* cur = m_elements[i];
			cur->add_position(glm::vec2(0.f, offset * SCROLL_MYLTIPLIER));
		}
	}
	void ScrollBox::add_element(GUI_element* element)
	{
		element->set_position(glm::vec2(m_position.x - m_scale.x, m_position.y - m_scale.y + 50.f)); // displacement X here
		element->set_scale(m_scale);
		for (size_t i = 0; i < m_elements.size(); i++)
		{
			m_elements[(i - m_elements.size() + 1) * -1]->set_position(
				glm::vec2(m_position.x - m_scale.x, (m_position.y - m_scale.y + 50.f) + ((i + 1) * 50.f))); // and here
		}
		m_elements.push_back(element);
		if (m_elements.size() > m_max_count_elements) m_elements.remove((size_t)0);
	}
	void ScrollBox::clear()
	{
		m_elements.clear();
	}
}