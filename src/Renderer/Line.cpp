#include "EngineCore/Renderer/Line.h";

#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/Renderer.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine
{
	Line::Line(std::shared_ptr<ShaderProgram> pShaderProgram, float size)
		: m_pShaderProgram(std::move(pShaderProgram))
		, m_size(size)
	{

		const GLfloat vertexCoords[] = {
			0.f, 0.f, 0.f,
			1.f, 1.f, 1.f
		};

		m_vertexCoordsBuffer.init(&vertexCoords, 3 * 2 * sizeof(GLfloat));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_vertexArray.unbind();
	}
	void Line::render(const glm::vec3& position, const glm::vec3& size, glm::vec3& color) const
	{
		m_pShaderProgram->use();

		glm::mat4 scaleMat(
			size.x, 0, 0, 0,
			0, size.y, 0, 0,
			0, 0, size.z, 0,
			0, 0, 0, 1);

		glm::mat4 translateMat(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			position.x, position.y, position.z, 1);

		glm::mat4 model = translateMat * scaleMat;

		m_pShaderProgram->setMatrix4("modelMat", model);
		m_pShaderProgram->setVec4("sourceColor", glm::vec4(color, 1.f));

		Renderer::drawLine(m_vertexArray, *m_pShaderProgram, m_size);
	}
}