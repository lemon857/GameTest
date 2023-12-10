#include "EngineCore/GUI/InputField.h"

#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/GUI/TextRenderer.h"
#include "EngineCore/GUI/Font.h"
#include "EngineCore/GUI/Sprite.h"

#include "EngineCore/Renderer/ShaderProgram.h"

#include "EngineCore/System/Log.h"
#include "EngineCore/System/Input.h"

#include "EngineCore/System/Keys.h"

#include <Windows.h>

GUI::InputField::InputField(Sprite* face, glm::vec2 pos, glm::vec2 scale,
	std::string name, std::shared_ptr<RenderEngine::ShaderProgram> shader,
	std::shared_ptr<Font> font, glm::vec3 textColor, bool clear_after_send, bool* focus_trigger)
	: GUI_element(name)
	, m_face(std::move(face))
	, m_textRenderer(new TextRenderer(std::move(font), std::move(shader), "",
		textColor, glm::vec2(pos.x - scale.x + SHIFT_TEXT_SYMBOL_X_P, pos.y - SHIFT_TEXT_SYMBOL_Y_P),
		scale, "default", false)) // font sclae here
	, m_isClicked(false)
	, m_clear_after_send(clear_after_send)
	, m_focus_trigger(std::move(focus_trigger))
{
	m_position_p = pos;
	m_scale_p = scale;
	m_face->set_position_p(pos);
	m_face->set_scale_p(scale);
}

GUI::InputField::~InputField()
{
}

void GUI::InputField::on_render_prj(glm::mat4& prj)
{

}

void GUI::InputField::on_press()
{
	m_isClicked = true;
	if (m_isFocused)
	{
		set_focus(false);
	}
	else
	{
		set_focus(true);
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
		set_focus(false);
	}
}
// need fix 
void GUI::InputField::set_active(const bool state)
{
	m_textRenderer->set_active(state);
	m_face->set_active(state);
	m_isActive = state;
}

std::vector<GUI::GUI_element*> GUI::InputField::get_elements()
{
	std::vector<GUI_element*> vec;
	vec.push_back(m_textRenderer);
	vec.push_back(m_face);
	return vec;
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
		if (m_focus_trigger != nullptr) *m_focus_trigger = true;
	}
	else
	{
		m_face->setSubTexture(NAME_TEXTURE_STATIC);
		if (m_focus_trigger != nullptr) *m_focus_trigger = false;
	}
}

void GUI::InputField::press_button(KeyCode key)
{
	if (!m_isFocused) return;
	if (key == KeyCode::KEY_V && Input::isKeyPressed(KeyCode::KEY_LEFT_CONTROL))
	{
		if (IsClipboardFormatAvailable(CF_TEXT))
		{
			if (OpenClipboard(0))
			{
				m_text += (char*)GetClipboardData(CF_TEXT);
				CloseClipboard();
			}
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

void GUI::InputField::press_char(char key_char)
{
	if (!m_isFocused) return;
	m_text += key_char;
	m_textRenderer->set_text(m_text);
}
