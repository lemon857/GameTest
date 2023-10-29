#pragma once 

#include <memory>
#include <string>
#include <glm/mat4x4.hpp>

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

	class FontRenderer
	{
	public:
		FontRenderer(std::shared_ptr<Font> font, std::shared_ptr<RenderEngine::ShaderProgram> shader);

		void render_text(std::string text, float x, float y, float scale, glm::vec3 color, glm::mat4& prj);

	private:
		std::shared_ptr<Font> m_font;
		std::shared_ptr<RenderEngine::ShaderProgram> m_shader;

		std::unique_ptr<RenderEngine::VertexArray> m_vertexArray;
		std::unique_ptr<RenderEngine::VertexBuffer> m_vertexCoordsBuffer;
		unsigned int VAO, VBO;
	};
}