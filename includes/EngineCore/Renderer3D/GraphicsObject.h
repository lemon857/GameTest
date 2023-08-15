#pragma once

#include <vector>
#include <memory>

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
		GraphicsObject(std::shared_ptr<ShaderProgram> m_pShaderProgram, std::vector<float>& vertexes, const std::vector<int>& indexes, const std::vector<float>& textures, const std::vector<float>& normales);

		void render(const glm::vec3& position, const glm::vec3& size);
	private:
		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		std::vector<float> m_textures;
		std::vector<float> m_normales;
		std::vector<float> m_vertexes;
		std::vector<int> m_indexes;
				
		VertexArray m_vao;
		VertexBuffer m_vbo_vert;
		VertexBuffer m_vbo_texture;
		VertexBuffer m_vbo_normal;
		IndexBuffer m_ebo;

		std::vector<GLfloat> vertex_coords;
		//GLfloat* normal_coords;
		//GLfloat* texture_coord;
		std::vector<GLuint> index_array;

	};
}