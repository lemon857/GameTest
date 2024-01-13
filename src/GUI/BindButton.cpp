#include "EngineCore/GUI/BindButton.h"

#include "EngineCore/GUI/GUI_place.h"
#include "EngineCore/System/Input.h"
#include "EngineCore/GUI/TextRenderer.h"
#include "EngineCore/GUI/Font.h"
#include "EngineCore/GUI/Sprite.h"

namespace GUI
{
	BindButton::BindButton(Sprite* face, glm::vec2 pos, glm::vec2 scale, 
		std::shared_ptr<RenderEngine::ShaderProgram> textShader, std::shared_ptr<Font> font, glm::vec3 textColor,
		KeyCode* targetBind, std::string name)
		: GUI_element(name)
		, m_face(std::move(face))
		, m_targetBind(std::move(targetBind))
		, m_textRenderer(new TextRenderer(font, std::move(textShader), Input::getKeyString(*m_targetBind), textColor, glm::vec2(pos.x, pos.y - SHIFT_TEXT_SYMBOL_Y), glm::vec2(0.5f), name + "text"))
	{
		m_position_p = pos;
		m_scale_p = scale;
		m_face->set_position_p(pos);
		m_face->set_scale_p(scale);
	}
	BindButton::~BindButton()
	{
	}
	void BindButton::on_press()
	{
		if (Input::getLastKeyPressed() != KeyCode::MOUSE_BUTTON_1)
		{
			return;
		}
		if (m_isClicked)
		{
			m_face->setSubTexture(NAME_TEXTURE_STATIC);
			m_isClicked = false;
		}
		else
		{
			m_face->setSubTexture(NAME_TEXTURE_CLICKED);
			m_isClicked = true;
		}
	}
	void BindButton::on_release()
	{
	}
	std::vector<GUI_element*> BindButton::get_elements()
	{
		add_tree_element(m_face);
		add_tree_element(m_textRenderer);
		m_textRenderer->set_layer(m_layer + 2);
		m_face->set_layer(m_layer);
		std::vector<GUI_element*> vec;
		vec.push_back(m_textRenderer);
		vec.push_back(m_face);
		return vec;
	}
	void BindButton::on_key_press(KeyCode key)
	{
		if (key == KeyCode::KEY_ESCAPE || key == KeyCode::MOUSE_BUTTON_1)
		{
			return;
		}
		if (m_isClicked)
		{
			*m_targetBind = key;
			m_textRenderer->set_text(Input::getKeyString(*m_targetBind));
			m_textRenderer->set_position(glm::vec2(m_position.x, m_position.y - GUI_place::get_pix_percent(glm::vec2(0.f, SHIFT_TEXT_SYMBOL_Y)).y));
			if ((int)key > 7)
			{
				m_face->setSubTexture(NAME_TEXTURE_STATIC);
				m_isClicked = false;
			}
		}
	}
}