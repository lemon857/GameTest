#include "EngineCore/GUI/InputField.h"

#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/GUI/TextRenderer.h"
#include "EngineCore/GUI/Font.h"
#include "EngineCore/GUI/Sprite.h"

#include "EngineCore/Renderer/ShaderProgram.h"

#include "EngineCore/System/Log.h"
#include "EngineCore/Input.h"

#include "EngineCore/Keys.h"

GUI::InputField::InputField(Sprite* face, glm::vec2 pos, glm::vec2 scale,
	std::string name, std::shared_ptr<RenderEngine::ShaderProgram> shader,
	std::shared_ptr<Font> font, glm::vec3 textColor, bool clear_after_send)
	: GUI_element(name)
	, m_face(std::move(face))
	, m_textRenderer(std::make_unique<TextRenderer>(std::move(font), std::move(shader), "", textColor, pos, glm::vec2(1.f)))
	, m_isFocused(false)
	, m_isClicked(false)
	, m_clear_after_send(clear_after_send)
{
	m_position_p = pos;
	m_scale_p = scale;
	m_face->set_position_p(pos);
	m_face->set_scale_p(scale);
	m_textRenderer->set_position(glm::vec2(pos.x, pos.y - SHIFT_TEXT_SYMBOL_Y));
}

GUI::InputField::~InputField()
{
}

void GUI::InputField::on_render_prj(glm::mat4& prj)
{
	if (!m_isActive) return;
	m_textRenderer->on_render_prj(prj);
	m_face->on_render_prj(prj);
}

void GUI::InputField::on_press()
{
	m_isClicked = true;
	if (m_isFocused)
	{
		m_face->setSubTexture(NAME_TEXTURE_STATIC);
		m_isFocused = false;
	}
	else
	{
		m_face->setSubTexture(NAME_TEXTURE_CLICKED);
		m_isFocused = true;
	}
}

void GUI::InputField::on_release()
{
	if (m_isClicked) // Flag for unfocus after clicking on any object
	{
		m_isClicked = false;
	}
	else
	{
		m_face->setSubTexture(NAME_TEXTURE_STATIC);
		m_isFocused = false;
	}
}

void GUI::InputField::set_position(glm::vec2 pos)
{
	m_face->set_position(pos);
	m_textRenderer->set_position(glm::vec2(pos.x, pos.y - SHIFT_TEXT_SYMBOL_Y));
	m_position = pos;
}

void GUI::InputField::set_scale(glm::vec2 scale)
{
	m_face->set_scale(scale);
	m_scale = scale;
}

std::string GUI::InputField::get_text()
{
	return m_text;
}

void GUI::InputField::set_text(std::string text)
{
	m_text = text;
	m_textRenderer->set_text(m_text);
}

bool GUI::InputField::get_focus()
{
	return m_isFocused;
}

void GUI::InputField::set_focus(bool focus)
{
	m_isFocused = focus;
	if (m_isFocused)
	{
		m_face->setSubTexture(NAME_TEXTURE_CLICKED);
	}
	else
	{
		m_face->setSubTexture(NAME_TEXTURE_STATIC);
	}
}

void GUI::InputField::press_button(KeyCode key)
{
	if (!m_isFocused) return;
	if (key <= KeyCode::KEY_Z)
	{
		if (key >= KeyCode::KEY_A)
		{
			m_text += (char)((int)key + (Input::isKeyPressed(KeyCode::KEY_LEFT_SHIFT) ? 0 : 32));
		}
		else
		{
			m_text += (char)key;
		}
	}
	else if (key == KeyCode::KEY_BACKSPACE && !m_text.empty())
	{
		m_text = m_text.substr(0, m_text.length() - 1);
	}
	else if (key == KeyCode::KEY_ENTER && !m_text.empty() && m_on_enter != nullptr)
	{
		m_on_enter(m_text);
		if (m_clear_after_send) m_text = "";
		else set_focus(false);
	}
	m_textRenderer->set_text(m_text);
}
