#include "Line.h";

#include "ShaderProgram.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine
{
	Line::Line(std::shared_ptr<ShaderProgram> pShaderProgram)
		: m_pShaderProgram(std::move(pShaderProgram))
	{

		const GLfloat vertexCoords[] = {
			0.f, 0.f,
			1.f, 1.f
		};

		m_vertexCoordsBuffer.init(&vertexCoords, 2 * 2 * sizeof(GLfloat));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_vertexArray.unbind();
	}
	void Line::render(const glm::vec2& position, const glm::vec2& size, const int layer, glm::vec4& color) const
	{
		m_pShaderProgram->use();

		glm::mat4 model(1.f);

		model = glm::translate(model, glm::vec3(position, 0.f));
		model = glm::scale(model, glm::vec3(size, 1.f));

		m_pShaderProgram->setMatrix4("modelMat", model);
		m_pShaderProgram->setFloat("layer", layer);
		m_pShaderProgram->setVec4("sourceColor", color);

		Renderer::drawLine(m_vertexArray, *m_pShaderProgram);
	}
}