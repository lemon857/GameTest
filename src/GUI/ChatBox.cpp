#include "EngineCore/GUI/ChatBox.h"

#include "EngineCore/GUI/Sprite.h"
#include "EngineCore/GUI/TextRenderer.h"

namespace GUI
{
	ChatBox::ChatBox(Sprite* backgrond, glm::vec2 pos, glm::vec2 scale,
		std::string name, int max_count_message, std::shared_ptr<Font> font,
		std::shared_ptr<RenderEngine::ShaderProgram> shader, glm::vec3 textColor)
		: GUI_element(name)
		, m_background(std::move(backgrond))
		, m_color(textColor)
		, m_font(std::move(font))
		, m_shader(std::move(shader))
		, m_max_count(max_count_message)
		, m_chat_messages()
		, m_isOpen(false)
	{
		m_position_p = pos;
		m_scale_p = scale;
		m_background->set_position_p(pos);
		m_background->set_scale_p(scale);
	}
	ChatBox::~ChatBox()
	{
		
	}
	void ChatBox::on_render_prj(glm::mat4& prj)
	{
		for (size_t i = 0; i < m_chat_messages.size(); i++)
		{
			m_chat_messages[i]->on_render_prj(prj);
		}
		if (m_isOpen) m_background->on_render_prj(prj);
	}
	void ChatBox::set_position(glm::vec2 pos)
	{
		m_background->set_position(pos);
		m_position = pos;
	}
	void ChatBox::set_scale(glm::vec2 scale)
	{
		m_background->set_scale(scale);
		m_scale = scale;
	}
	void ChatBox::add_message(std::string message)
	{
		if (m_chat_messages.size() >= m_max_count)
		{
			m_chat_messages.remove((size_t)0); // need fix memory leak
		}
		auto text = new TextRenderer(m_font, m_shader, message, m_color, m_position, glm::vec2(1.f), "default", false);
		text->set_position(glm::vec2(m_position.x - m_scale.x, (m_position.y - m_scale.y) + 20.f));
		text->set_scale(m_scale);
		for (size_t i = 0; i < m_chat_messages.size(); i++)
		{
			m_chat_messages[(i - m_chat_messages.size() + 1) * -1]->set_position(glm::vec2(m_position.x - m_scale.x, (m_position.y - m_scale.y) + 20.f + ((i + 1) * 50.f)));
		}
		m_chat_messages.push_back(text);		
	}
	void ChatBox::clear()
	{
		m_chat_messages.clear();
	}
}