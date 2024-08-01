#include "EngineCore/GUI/Table.h"

#include "EngineCore/GUI/Sprite.h"
#include "EngineCore/GUI/TextRenderer.h"

#define SHIFT_TABLE_X 4.7f
#define SHIFT_TABLE_Y 4.f

namespace GUI
{
	Table::Table(Sprite* backgrond, glm::vec2 pos, glm::vec2 scale, glm::vec2 scale_cell, 
		std::string name, Font* font, RenderEngine::ShaderProgram* shader, glm::vec3 textColor,
		int colls, int rows, std::vector<std::string> data)
		: GUI_element(name)
		, m_background(std::move(backgrond))
	{
		m_position_p = pos;
		m_scale_p = scale;
		m_background->set_position_p(pos);
		m_background->set_scale_p(scale);
		float x_offset = pos.x - scale.x + SHIFT_TABLE_X;
		float y_offset = pos.y + scale.y - SHIFT_TABLE_Y;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < colls; j++)
			{
				m_texts.push_back(new TextRenderer(font, shader, data[j + i * colls], textColor, glm::vec2(x_offset, y_offset), glm::vec2(1.f)));
				x_offset += scale_cell.x;
			}
			x_offset = pos.x - scale.x + SHIFT_TABLE_X;
			y_offset -= scale_cell.y;
		}
	}
	Table::Table(Sprite* backgrond, glm::vec2 pos, glm::vec2 scale, glm::vec2 scale_cell,
		std::string name, Font* font, RenderEngine::ShaderProgram* shader, glm::vec3 textColor,
		int colls, int rows, std::vector<std::wstring> data)
		: GUI_element(name)
		, m_background(std::move(backgrond))
	{
		m_position_p = pos;
		m_scale_p = scale;
		m_background->set_position_p(pos);
		m_background->set_scale_p(scale);
		float x_offset = pos.x - scale.x + SHIFT_TABLE_X;
		float y_offset = pos.y + scale.y - SHIFT_TABLE_Y;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < colls; j++)
			{
				m_texts.push_back(new TextRenderer(font, shader, data[j + i * colls], textColor, glm::vec2(x_offset, y_offset), glm::vec2(1.f)));
				x_offset += scale_cell.x;
			}
			x_offset = pos.x - scale.x + SHIFT_TABLE_X;
			y_offset -= scale_cell.y;
		}
	}
	Table::~Table()
	{
	}
	void Table::on_render_prj(glm::mat4 prj)
	{

	}
	std::vector<GUI_element*> Table::get_elements()
	{
		m_texts.push_back(m_background);
		return m_texts;
	}
}
