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

	class TextRenderer : public GUI_element
	{
	public:
		TextRenderer(std::shared_ptr<Font> font, std::shared_ptr<RenderEngine::ShaderProgram> shader,
			std::string text, glm::vec3 color, glm::vec2 pos, glm::vec2 scale,
			std::string name = "default", bool isCenterCoord = true);

		TextRenderer(std::shared_ptr<Font> font, std::shared_ptr<RenderEngine::ShaderProgram> shader,
			std::wstring text, glm::vec3 color, glm::vec2 pos, glm::vec2 scale,
			std::string name = "default", bool isCenterCoord = true);

		void render_text(std::wstring text, float x, float y, float scale, glm::vec3 color, float layer, glm::mat4& prj);

		void on_render_prj(glm::mat4& prj);

		void set_text(std::string text);
		void set_text(std::wstring text);

		void set_position(glm::vec2 pos) override;
	private:
		std::shared_ptr<Font> m_font;
		std::shared_ptr<RenderEngine::ShaderProgram> m_shader;

		std::wstring m_text;
		glm::vec3 m_color;

		glm::vec2 m_p_pos;

		std::unique_ptr<RenderEngine::VertexArray> m_vertexArray;
		RenderEngine::VertexBuffer* m_vertexCoordsBuffer;
		unsigned int VAO, VBO;
		bool m_isCenterCoords;
	};
}