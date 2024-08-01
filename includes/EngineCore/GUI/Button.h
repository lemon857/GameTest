#pragma once

#include "EngineCore/GUI/GUI_element.h"
#include <string>
#include <memory>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

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

	class Button : public GUI_element
	{
	public:
		Button(Sprite* face, glm::vec2 pos, glm::vec2 scale,
			std::string text, RenderEngine::ShaderProgram* textShader, 
			Font* font, glm::vec3 textColor, std::string name = "default");

		Button(Sprite* face, glm::vec2 pos, glm::vec2 scale,
			std::wstring text, RenderEngine::ShaderProgram* textShader,
			Font* font, glm::vec3 textColor, std::string name = "default");
		~Button();

		void on_render_prj(glm::mat4 prj) override;

		void on_press() override;
		void on_release() override;
		void on_release_hover() override;

		std::vector<GUI_element*> get_elements() override;

		void set_text(std::string text);
		void set_text(std::wstring text);

	private:
		Sprite* m_face;
		TextRenderer* m_textRenderer;
		bool m_isClicked = false;
	};
}
