#pragma once

#include "EngineCore/GUI/GUI_element.h"

#include "EngineCore/System/List.h"

#include <string>
#include <glm/vec3.hpp>

namespace RenderEngine
{
	class Material;
	class ShaderProgram;
}
namespace GUI
{
	class TextRenderer;
	class Font;
	class Sprite;
	class ScrollBox;

	class ChatBox : public GUI_element
	{
	public:
		ChatBox(Sprite* backgrond, glm::vec2 pos, glm::vec2 scale,
			std::string name, int max_count_message, 
			std::shared_ptr<Font> font, std::shared_ptr<RenderEngine::ShaderProgram> shader, glm::vec3 textColor);
		~ChatBox();

		void on_render_prj(glm::mat4& prj) override;

		void set_position(glm::vec2 pos) override;
		void set_scale(glm::vec2 scale) override;

		void add_message(std::string message);

		void set_open(bool isOpen) { m_isOpen = isOpen; }

		void clear();

		void on_scroll(int offset);
	private:
		Sprite* m_background;
		ScrollBox* m_scrollbox;

		bool m_isOpen;

		std::shared_ptr<Font> m_font;
		std::shared_ptr<RenderEngine::ShaderProgram> m_shader;
		glm::vec3 m_color;
	};
}