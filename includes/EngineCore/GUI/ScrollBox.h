#pragma once

#include "EngineCore/GUI/GUI_element.h"

#include "EngineCore/System/List.h"

#include <string>
#include <glm/vec3.hpp>
namespace GUI
{
	class Sprite;
	class ScrollBox : public GUI_element
	{
	public:
		ScrollBox(Sprite* backgrond, glm::vec2 pos, glm::vec2 scale,
			std::string name, int max_count_elements);
		~ScrollBox();

		void on_render_prj(glm::mat4& prj) override;

		void set_position(glm::vec2 pos) override;
		void set_scale(glm::vec2 scale) override;

		void on_scroll(int offset);

		void add_element(GUI_element* element);

		void clear();
	private:
		unsigned int m_max_count_elements;

		linked_list<GUI_element*> m_elements;
		Sprite* m_background;
	};
}