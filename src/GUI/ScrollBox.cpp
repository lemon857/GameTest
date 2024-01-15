#include "EngineCore/GUI/ScrollBox.h"

#include "EngineCore/GUI/Sprite.h"
#include "EngineCore/GUI/GUI_place.h"

#define SCROLL_MYLTIPLIER 10

#define DISPLACEMENT_ELEMENT_Y 25.f
#define SHIFT_ELEMENT_Y 30.f

namespace GUI
{
	ScrollBox::ScrollBox(Sprite* backgrond, glm::vec2 pos, glm::vec2 scale, std::string name, int max_count_elements, bool has_shift, GUI_place* place, bool isHorisontal)
		: GUI_element(name)
		, m_max_count_elements(max_count_elements)
		, m_background(std::move(backgrond))
		, m_place(std::move(place))
		, m_has_shift(has_shift)
		, m_isHorisontal(isHorisontal)
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
	void ScrollBox::on_render_prj(glm::mat4 prj)
	{
		for (size_t i = 0; i < m_elements.size(); i++)
		{
			GUI_element* cur = m_elements[i];
			if (cur->get_position().y < m_scale.y + m_position.y - (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().y)
				&& cur->get_position().y > m_position.y - m_scale.y + (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().y)) cur->set_active(true);
			else cur->set_active(false);
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
	void ScrollBox::on_mouse_scroll(int offset)
	{
		if (!m_isActive) return;
		if (m_elements.size() < 2) return;
		if (m_isHorisontal)
		{

		}
		else
		{
			if (((m_elements[0]->get_position().y < m_position.y + m_scale.y - m_elements[0]->get_scale().y - m_has_shift ? SHIFT_ELEMENT_Y : 0) && offset < 0) ||
				((m_elements.last()->get_position().y > m_position.y - m_scale.y + m_elements[0]->get_scale().y + m_has_shift ? SHIFT_ELEMENT_Y : 0) && offset > 0))
				return;
		}
		if (m_isHorisontal)
		{
			for (size_t i = 0; i < m_elements.size(); i++)
			{
				GUI_element* cur = m_elements[i];
				cur->add_position(glm::vec2(offset * SCROLL_MYLTIPLIER, 0.f));
				if (cur->get_position().x < m_scale.x + m_position.x - (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().x)
					&& cur->get_position().x > m_position.x - m_scale.x + (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().x)) cur->set_active(true);
				else cur->set_active(false);
			}
		}
		else
		{
			for (size_t i = 0; i < m_elements.size(); i++)
			{
				GUI_element* cur = m_elements[i];
				cur->add_position(glm::vec2(0.f, offset * SCROLL_MYLTIPLIER));
				if (cur->get_position().y < m_scale.y + m_position.y - (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().y)
					&& cur->get_position().y > m_position.y - m_scale.y + (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().y)) cur->set_active(true);
				else cur->set_active(false);
			}
		}
	}
	void ScrollBox::set_active(const bool state)
	{
		m_isActive = state;
		set_tree_active(state);
		/*for (size_t i = 0; i < m_elements.size(); i++)
		{
			GUI_element* cur = m_elements[i];
			if (cur->get_position().y < m_scale.y + m_position.y - (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().y)
				&& cur->get_position().y > m_position.y - m_scale.y + (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().y)) cur->set_active(true);
			else cur->set_active(false);
		}*/
		//on_mouse_scroll(10);
		//on_mouse_scroll(-1);
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
				glm::vec2(m_position.x - m_scale.x, (m_position.y - m_scale.y + SHIFT_ELEMENT_Y) + (( i + 1) * SHIFT_ELEMENT_Y)));
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
		/*for (size_t i = 0; i < m_elements.size(); i++)
		{
			GUI_element* cur = m_elements[i];
			if (cur->get_position().y < m_scale.y + m_position.y - (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().y)
				&& cur->get_position().y > m_position.y - m_scale.y + (m_has_shift ? DISPLACEMENT_ELEMENT_Y : cur->get_scale().y)) cur->set_active(true);
			else cur->set_active(false);
		}*/
		//on_mouse_scroll(1);
		//on_mouse_scroll(-1);
	}
}