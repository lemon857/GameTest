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
	class ShaderProrgam;
}

namespace GUI
{
	class FontRenderer;
	class Font;
	class Sprite;

	class Button : public GUI_element
	{
	public:
		Button(Sprite* face, std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec2 pos, glm::vec2 scale,
			std::string text, std::string shaderName, std::shared_ptr<Font> font, glm::vec3 textColor);
		~Button();

		void on_render_prj(glm::mat4& prj) override;

		void on_press() override;
		void on_release() override;

		void set_position(glm::vec2 pos) override;
		void set_scale(glm::vec2 scale) override;



	private:
		Sprite* m_face;
		std::unique_ptr<FontRenderer> m_textRenderer;
		bool m_isClicked = false;
		int m_text_length = 0;
	};
}