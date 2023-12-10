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
		, m_color(textColor)
		, m_font(std::move(font))
		, m_shader(std::move(shader))
		, m_scrollbox(new ScrollBox(std::move(backgrond), pos, scale, "scrollbox", max_count_message))
	{
		m_position_p = pos;
		m_scale_p = scale;
	}
	ChatBox::~ChatBox()
	{

	}
	void ChatBox::on_render_prj(glm::mat4& prj)
	{

	}
	std::vector<GUI_element*> ChatBox::get_elements()
	{
		std::vector<GUI_element*> vec;
		vec.push_back(m_scrollbox);
		return vec;
	}
	void ChatBox::add_message(std::string message)
	{
		m_scrollbox->add_element(new TextRenderer(m_font, m_shader, message, m_color, m_position, glm::vec2(0.3f), "default", false)); // font scale here
	}
	void ChatBox::set_open(bool isOpen)
	{
		m_scrollbox->set_open(isOpen);
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