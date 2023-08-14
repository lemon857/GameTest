#pragma once

#include <memory>
#include <string>
#include <glm/vec3.hpp>
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
		Line(std::shared_ptr<ShaderProgram> pShaderProgram, float size = 1.f);

		Line(const Line&) = delete;
		Line& operator=(const Line&) = delete;

		void render(const glm::vec3& position, const glm::vec3& size, glm::vec3& color) const;
	private:
		std::shared_ptr<ShaderProgram> m_pShaderProgram;
		float m_size;

		VertexArray m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
	};
}