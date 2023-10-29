#pragma once

#include "EngineCore/GUI/GUI_element.h"
#include <string>
#include <memory>

namespace RenderEngine
{
	class Material;
	class ShaderProrgam;
}

namespace GUI
{
	class FontRenderer;
	class Font;

	class Button : public GUI_element
	{
	public:
		Button(GUI_element face, std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec2 pos, glm::vec2 scale, 
			std::string text, std::string shaderName, std::shared_ptr<Font> font);
		~Button();

		void on_render() override;

	private:
		std::string m_text;
		std::unique_ptr<FontRenderer> m_textRenderer;
	};
}