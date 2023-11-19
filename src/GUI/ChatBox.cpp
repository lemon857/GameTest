#include "EngineCore/GUI/ChatBox.h"

#include "EngineCore/GUI/Sprite.h"
#include "EngineCore/GUI/TextRenderer.h"
#include "EngineCore/GUI/ScrollBox.h"

namespace GUI
{
	ChatBox::ChatBox(Sprite* backgrond, glm::vec2 pos, glm::vec2 scale,
		std::string name, int max_count_message, std::shared_ptr<Font> font,
		std::shared_ptr<RenderEngine::ShaderProgram> shader, glm::vec3 textColor)
		: GUI_element(name)
		, m_background(backgrond)
		, m_color(textColor)
		, m_font(std::move(font))
		, m_shader(std::move(shader))
		, m_isOpen(false)
		, m_scrollbox(new ScrollBox(std::move(backgrond), pos, scale, "scrollbox", max_count_message))
	{
		m_position_p = pos;
		m_scale_p = scale;
		m_background->set_position_p(pos);
		m_background->set_scale_p(scale);
		m_scrollbox->set_position_p(pos);
		m_scrollbox->set_scale_p(scale);
	}
	ChatBox::~ChatBox()
	{
		delete m_scrollbox;		
	}
	void ChatBox::on_render_prj(glm::mat4& prj)
	{
		m_scrollbox->on_render_prj(prj);
		if (m_isOpen) m_background->on_render_prj(prj);
	}
	void ChatBox::set_position(glm::vec2 pos)
	{
		m_background->set_position(pos);
		m_scrollbox->set_position(pos);
		m_position = pos;
	}
	void ChatBox::set_scale(glm::vec2 scale)
	{
		m_background->set_scale(scale);
		m_scrollbox->set_scale(scale);
		m_scale = scale;
	}
	void ChatBox::add_message(std::string message)
	{
		m_scrollbox->add_element(new TextRenderer(m_font, m_shader, message, m_color, m_position, glm::vec2(1.f), "default", false));
	}
	void ChatBox::clear()
	{
		m_scrollbox->clear();
	}
	void ChatBox::on_scroll(int offset)
	{
		m_scrollbox->on_scroll(offset);
	}
}