#include "EngineCore/Renderer3D/GraphicsObject.h"

#include "EngineCore/Renderer/Renderer.h"

#include <array>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine
{

	GraphicsObject::GraphicsObject(std::shared_ptr<ShaderProgram> pShaderProgram, std::vector<float>& vertexes, const std::vector<int>& indexes, const std::vector<float>& textures, const std::vector<float>& normales)
		: m_vertexes(vertexes)
		, m_indexes(indexes)
		, m_normales(normales)
		, m_textures(textures)
		, m_pShaderProgram(std::move(pShaderProgram))
	{
		index_array.reserve(indexes.size() / 3);
		//normal_coords = new GLfloat[indexes.size() / 3];
		//texture_coord = new GLfloat[(indexes.size() * 2) / 3];

		for (size_t i = 0; i < vertexes.size(); i++)
		{
			vertex_coords.push_back(vertexes[i]);
		}

		for (size_t i = 0; i < indexes.size() / 3; i++)
		{
			index_array[i] = indexes[i * 3];
			//texture_coord[i] = textures[indexes[1 + i * 3]];
			//normal_coords[i] = normales[indexes[2 + i * 3]];
		}

		m_vbo_vert.init(&vertex_coords, vertexes.size() * sizeof(GLfloat));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		m_vao.addBuffer(m_vbo_vert, vertexCoordsLayout);

		m_ebo.init(index_array.data(), sizeof(index_array[0]) / sizeof(index_array));

		m_vao.unbind();
		m_ebo.unbind();
	}
	void GraphicsObject::render(const glm::vec3& position, const glm::vec3& size)
	{
		glm::mat4 model(1.f);

		model = glm::translate(model, position);
		//model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
		//model = glm::rotate(model, static_cast<float>(glm::radians(rotation)),  glm::vec3(0.f, 0.f, 1.f));
		//model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f));
		model = glm::scale(model, size);

		m_pShaderProgram->setMatrix4("modelMat", model);
		Renderer::drawTriangles(m_vao, m_ebo);
	}
}