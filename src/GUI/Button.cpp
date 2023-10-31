#include "EngineCore/GUI/Button.h"

#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/GUI/FontRenderer.h"
#include "EngineCore/GUI/Font.h"
#include "EngineCore/GUI/Sprite.h"

#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Resources/ResourceManager.h"

#define SHIFT_TEXT_SYMBOL_X 20.5f
#define SHIFT_TEXT_SYMBOL_Y 8.f

#define NAME_TEXTURE_STATIC "static"
#define NAME_TEXTURE_CLICKED "clicked"

namespace GUI
{
	Button::Button(Sprite* face, std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec2 pos, glm::vec2 scale,
		std::string text, std::string shaderName, std::shared_ptr<Font> font, glm::vec3 textColor)
		: GUI_element(std::move(pMaterial), "button")
		, m_textRenderer(std::make_unique<FontRenderer>(font, ResourceManager::getShaderProgram(shaderName), text, textColor, pos, glm::vec2(1.f)))
		, m_face(std::move(face))
	{
		m_position_p = pos;
		m_scale_p = scale;
		m_text_length = text.length();
		m_face->set_position_p(pos);
		m_face->set_scale_p(scale);
		m_textRenderer->set_position(glm::vec2(pos.x - ((m_text_length / 2) * SHIFT_TEXT_SYMBOL_X), pos.y - SHIFT_TEXT_SYMBOL_Y));
	}
	Button::~Button()
	{
		
	}
	void Button::on_render_prj(glm::mat4& prj)
	{
		m_textRenderer->on_render_prj(prj);
		m_face->on_render_prj(prj);
	}
	void Button::on_press()
	{
		m_face->setSubTexture(NAME_TEXTURE_CLICKED);
		m_isClicked = true;
	}
	void Button::on_release()
	{
		m_face->setSubTexture(NAME_TEXTURE_STATIC);
		if (m_isClicked)
		{
			on_click();
			m_isClicked = false;
		}
	}
	void Button::set_position(glm::vec2 pos)
	{
		m_face->set_position(pos);
		m_textRenderer->set_position(glm::vec2(pos.x - ((m_text_length / 2) * SHIFT_TEXT_SYMBOL_X), pos.y - SHIFT_TEXT_SYMBOL_Y));
		m_position = pos;
	}
	void Button::set_scale(glm::vec2 scale)
	{
		m_face->set_scale(scale);
		m_scale = scale;
	}
}