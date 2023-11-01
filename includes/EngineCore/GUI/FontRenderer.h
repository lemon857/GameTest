#pragma once 

#include <memory>
#include <string>
#include <glm/mat4x4.hpp>

#include "EngineCore/GUI/GUI_element.h"

namespace RenderEngine
{
	class ShaderProgram;
	class Material;
	class VertexBuffer;
	class VertexArray;
	class IndexBuffer;
}

namespace GUI
{
	class Font;

	class FontRenderer : public GUI_element
	{
	public:
		FontRenderer(std::shared_ptr<Font> font, std::shared_ptr<RenderEngine::ShaderProgram> shader, std::string text, glm::vec3 color, glm::vec2 pos, glm::vec2 scale);

		void render_text(std::string text, float x, float y, float scale, glm::vec3 color, glm::mat4& prj);

		void on_render_prj(glm::mat4& prj);

	private:
		std::shared_ptr<Font> m_font;
		std::shared_ptr<RenderEngine::ShaderProgram> m_shader;

		std::string m_text;
		glm::vec3 m_color;

		std::unique_ptr<RenderEngine::VertexArray> m_vertexArray;
		RenderEngine::VertexBuffer* m_vertexCoordsBuffer;
		unsigned int VAO, VBO;
	};
}