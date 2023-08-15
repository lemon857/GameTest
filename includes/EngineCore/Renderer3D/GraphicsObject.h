#pragma once

#include <vector>
#include <memory>
#include <string>

#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/IndexBuffer.h"

#include <glm/vec3.hpp>

namespace RenderEngine
{
	class ShaderProgram;

	class GraphicsObject
	{
	public:
		GraphicsObject(std::shared_ptr<ShaderProgram> pShaderProgram, const std::string& filePath);

		void render(const glm::vec3& position, const glm::vec3& size);
	private:
		std::shared_ptr<ShaderProgram> m_pShaderProgram;
						
		VertexArray m_vao;
		VertexBuffer m_vbo_vert;
		VertexBuffer m_vbo_texture;
		VertexBuffer m_vbo_normal;
		IndexBuffer m_ebo;

		std::vector<GLfloat> m_vertex_coords;
		std::vector<GLfloat> m_normal_coords;
		std::vector<GLfloat> m_texture_coord;
		std::vector<GLuint> m_index_array;

	};
}