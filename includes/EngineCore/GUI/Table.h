#pragma once

#include "EngineCore/GUI/GUI_element.h"

#include <string>
#include <vector>
#include <glm/vec3.hpp>

namespace RenderEngine
{
	class Material;
	class ShaderProgram;
}

namespace GUI
{
	class Font;
	class Sprite;
	
	class Table : public GUI_element
	{
	public:
		Table(Sprite* backgrond, glm::vec2 pos, glm::vec2 scale,
			glm::vec2 scale_cell, std::string name,
			Font* font, RenderEngine::ShaderProgram* shader,
			glm::vec3 textColor, int colls, int rows, std::vector<std::string> data);
		Table(Sprite* backgrond, glm::vec2 pos, glm::vec2 scale,
			glm::vec2 scale_cell, std::string name,
			Font* font, RenderEngine::ShaderProgram* shader,
			glm::vec3 textColor, int colls, int rows, std::vector<std::wstring> data);
		~Table();

		void on_render_prj(glm::mat4 prj) override;

		std::vector<GUI_element*> get_elements() override;
	private:
		Sprite* m_background;
		std::vector<GUI_element*> m_texts;
	};
}
