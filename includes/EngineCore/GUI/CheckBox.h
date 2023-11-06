#pragma once

#include "EngineCore/GUI/GUI_element.h"

#include <string>

namespace RenderEngine
{
	class Material;
}

namespace GUI
{
	class TextRenderer;
	class Font;
	class Sprite;

	class CheckBox : public GUI_element
	{
	public:
		CheckBox(Sprite* backgroung, Sprite* checkFlag, glm::vec2 pos, glm::vec2 scale, std::string name);
		~CheckBox();

		void on_render_prj(glm::mat4& prj) override;

		void on_press() override;
		void on_release() override;

		void set_position(glm::vec2 pos) override;
		void set_scale(glm::vec2 scale) override;
	private:
		Sprite* m_backgroung;
		Sprite* m_check;
		bool m_value;
		bool m_isClicked;
	};
}