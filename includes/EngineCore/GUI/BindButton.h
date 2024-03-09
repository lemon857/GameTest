#pragma once

#include "EngineCore/GUI/GUI_element.h"
#include <string>
#include <memory>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

enum class KeyCode;

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

	class BindButton : public GUI_element
	{
	public:
		BindButton(Sprite* face, glm::vec2 pos, glm::vec2 scale,
			 RenderEngine::ShaderProgram* textShader, Font* font, glm::vec3 textColor, KeyCode* targetBind, std::string name = "default");
		~BindButton();

		void on_press() override;
		void on_release() override;

		std::vector<GUI_element*> get_elements() override;

		void on_key_press(KeyCode key) override;
	private:
		Sprite* m_face;
		KeyCode* m_targetBind;
		TextRenderer* m_textRenderer;
		KeyCode m_last;
		bool m_isClicked = false;
	};
}