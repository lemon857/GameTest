#include "EngineCore/GUI/ScrollBox.h"

#include "EngineCore/GUI/Sprite.h"
#include "EngineCore/GUI/GUI_place.h"

#define SCROLL_MYLTIPLIER 10

#define DISPLACEMENT_ELEMENT_Y 25.f
#define SHIFT_ELEMENT_Y 30.f

namespace GUI
{
	ScrollBox::ScrollBox(Sprite* backgrond, glm::vec2 pos, glm::vec2 scale, std::string name, int max_count_elements, bool has_shift, GUI_place* place)
		: GUI_element(name)
		, m_max_count_elements(max_count_elements)
		, m_background(std::move(backgrond))
		, m_place(std::move(place))
		, m_has_shift(has_shift)
	{
		m_position_p = pos;
		m_scale_p = scale;
		m_background->set_position_p(pos);
		m_background->set_scale_p(scale);
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
			if (cur->get_position().y < m_scale.y + m_position.y - DISPLACEMENT_ELEMENT_Y && cur->get_position().y > m_position.y - m_scale.y + DISPLACEMENT_ELEMENT_Y)
			{
				cur->set_active(true);
			}
			else 
			{
				cur->set_active(false);
			}
		}
	}
	// need fix for add elements in render list in gui place
	std::vector<GUI_element*> ScrollBox::get_elements()
	{
		add_tree_element(m_background);
		m_background->set_layer(m_layer);
		std::vector<GUI_element*> vec;
		vec.push_back(m_background);
		return vec;
	}
	// need fix overscrolling
	void ScrollBox::on_scroll(int offset)
	{
		if (!m_isActive && m_isFocused) return;
		for (size_t i = 0; i < m_elements.size(); i++)
		{
			m_elements[i]->add_position(glm::vec2(0.f, offset * SCROLL_MYLTIPLIER));
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
			element->set_position(glm::vec2(m_position.x, m_position.y - m_scale.y + 110.f)); // displacement X here	
		}
		for (size_t i = 0; i < m_elements.size(); i++)
		{
			if (m_has_shift)
				m_elements[(i - m_elements.size() + 1) * -1]->set_position(
				glm::vec2(m_position.x - m_scale.x, (m_position.y - m_scale.y + SHIFT_ELEMENT_Y) + ((i + 1) * SHIFT_ELEMENT_Y)));
			else
				m_elements[(i - m_elements.size() + 1) * -1]->set_position(
				glm::vec2(m_position.x, (m_position.y - m_scale.y + 110.f) + ((i + 1) * 110.f)));
		}
		add_tree_element(element);
		m_elements.push_back(element);
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