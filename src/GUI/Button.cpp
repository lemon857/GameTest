#include "EngineCore/GUI/Button.h"

#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/GUI/TextRenderer.h"
#include "EngineCore/GUI/Font.h"
#include "EngineCore/GUI/Sprite.h"

#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Resources/ResourceManager.h"

namespace GUI
{
	Button::Button(Sprite* face, glm::vec2 pos, glm::vec2 scale,
		std::string text, std::string shaderName, std::shared_ptr<Font> font, glm::vec3 textColor)
		: GUI_element(face->get_material(), text)
		, m_textRenderer(std::make_unique<TextRenderer>(font, ResourceManager::getShaderProgram(shaderName), text, textColor, pos, glm::vec2(1.f)))
		, m_face(std::move(face))
	{
		m_position_p = pos;
		m_scale_p = scale;
		m_face->set_position_p(pos);
		m_face->set_scale_p(scale);
		m_textRenderer->set_position(glm::vec2(pos.x, pos.y - SHIFT_TEXT_SYMBOL_Y));
	}
	Button::~Button()
	{
		delete m_face;
	}
	void Button::on_render_prj(glm::mat4& prj)
	{
		if (!m_isActive) return;
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
		m_textRenderer->set_position(glm::vec2(pos.x, pos.y - SHIFT_TEXT_SYMBOL_Y));
		m_position = pos;
	}
	void Button::set_scale(glm::vec2 scale)
	{
		m_face->set_scale(scale);
		m_scale = scale;
	}
}