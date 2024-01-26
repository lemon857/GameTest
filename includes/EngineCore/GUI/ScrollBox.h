#pragma once

#include "EngineCore/GUI/GUI_element.h"

#include "EngineCore/System/List.h"

#include <string>
#include <glm/vec3.hpp>

namespace RenderEngine
{
	class Line;
	class Material;
}

namespace GUI
{
	class GUI_place;
	class Sprite;
	class ScrollBox : public GUI_element
	{
	public:
		// if material equals nullptr line doesn't use
		ScrollBox(Sprite* backgrond, glm::vec2 pos, glm::vec2 scale,
			std::string name, int max_count_elements, std::shared_ptr<RenderEngine::Material> pMaterialLine = nullptr,
			bool has_displacement = false, GUI_place* place = nullptr, bool isHorisontal = false, bool right_line = true, float percentDisplaysment = 0.15f);
		~ScrollBox();

		void on_render_prj(glm::mat4& prj) override;

		std::vector<GUI_element*> get_elements() override;

		void on_mouse_scroll(int offset) override;

		void set_active(const bool state) override;

		void set_position(glm::vec2 pos) override;

		void add_element(GUI_element* element);

		void clear();

		void set_open(bool isOpen);
	private:
		bool m_has_shift;
		bool m_isHorisontal;
		bool m_onLine;
		bool m_right_line;
		float m_perc_disp;
		float m_cur_scroll;
		float m_disp_scroll;
		unsigned int m_max_count_elements;
		int m_count_elements;
		GUI_place* m_place;
		linked_list<GUI_element*> m_elements;
		Sprite* m_background;
		RenderEngine::Line* m_line;
		glm::vec3 m_pos_line;
	};
}