#pragma once

#include <memory>
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace RenderEngine 
{
	class Texture2D;
	class ShaderProgram;

	class Line 
	{
	public:
		Line(std::shared_ptr<ShaderProgram> pShaderProgram);

		Line(const Line&) = delete;
		Line& operator=(const Line&) = delete;

		void render(const glm::vec2& position, const glm::vec2& size, const int layer, glm::vec4& color) const;
	private:
		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
	};
}