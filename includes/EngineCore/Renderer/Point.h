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

	class Point 
	{
	public:
		Point(std::shared_ptr<ShaderProgram> pShaderProgram, float size = 1.f);

		Point(const Point&) = delete;
		Point& operator=(const Point&) = delete;

		void render(const glm::vec2 position, const int layer, glm::vec4& color) const;
	private:
		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
		float m_size;
	};
}