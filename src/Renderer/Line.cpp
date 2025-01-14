#include "EngineCore/Renderer/Line.h"

#include "EngineCore/System/ShadersSettings.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexArray.h"

namespace RenderEngine
{
	Line::Line(Material* pMaterial, float size)
		: m_pMaterial(std::move(pMaterial))
		, m_size(size)
	{

		const GLfloat vertexCoords[] = {
			0.f, 0.f, 0.f,
			1.f, 1.f, 1.f
		};

		m_vertexCoordsBuffer = new VertexBuffer();
		m_vertexArray = new VertexArray();

		m_vertexCoordsBuffer->init(&vertexCoords, 3 * 2 * sizeof(GLfloat), true);
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		m_vertexArray->addBuffer(*m_vertexCoordsBuffer, vertexCoordsLayout);

		m_vertexArray->unbind();
	}
	Line::~Line()
	{
		m_vertexCoordsBuffer->unbind();
		m_vertexArray->unbind();
		delete m_vertexCoordsBuffer;
		delete m_vertexArray;
	}
	void Line::render(const glm::vec3 position, const glm::vec3 dir, glm::vec3 color) const
	{
		m_pMaterial->use();

		glm::mat4 scaleMat(
			dir.x, 0, 0, 0,
			0, dir.y, 0, 0,
			0, 0, dir.z, 0,
			0, 0, 0, 1);

		glm::mat4 translateMat(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			position.x, position.y, position.z, 1);

		glm::mat4 model = translateMat * scaleMat;

		m_pMaterial->set_model_matrix(model);
		m_pMaterial->get_shader_ptr()->setVec4(SS_COLOR_PROP_NAME, glm::vec4(color, 1.f));

		Renderer::drawLine(*m_vertexArray, m_size);
	}
	void Line::render_from_to(const glm::vec3 position, const glm::vec3 to, glm::vec3 color) const
	{
		glm::vec3 dir = to - position;

		glm::mat4 scaleMat(
			dir.x, 0, 0, 0,
			0, dir.y, 0, 0,
			0, 0, dir.z, 0,
			0, 0, 0, 1);

		glm::mat4 translateMat(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			position.x, position.y, position.z, 1);

		glm::mat4 model = translateMat * scaleMat;

		m_pMaterial->use();
		m_pMaterial->set_model_matrix(model);
		m_pMaterial->get_shader_ptr()->setVec4(SS_COLOR_PROP_NAME, glm::vec4(color, 1.f));

		Renderer::drawLine(*m_vertexArray, m_size);
	}
	void Line::render(const glm::vec3 position, const glm::vec3 dir, glm::vec4 color) const
	{
		glm::mat4 scaleMat(
			dir.x, 0, 0, 0,
			0, dir.y, 0, 0,
			0, 0, dir.z, 0,
			0, 0, 0, 1);

		glm::mat4 translateMat(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			position.x, position.y, position.z, 1);

		glm::mat4 model = translateMat * scaleMat;

		m_pMaterial->use();
		m_pMaterial->set_model_matrix(model);
		m_pMaterial->get_shader_ptr()->setVec4(SS_COLOR_PROP_NAME, color);

		Renderer::drawLine(*m_vertexArray, m_size);
	}
	void Line::render_from_to(const glm::vec3 position, const glm::vec3 to, glm::vec4 color) const
	{
		glm::vec3 dir = to - position;

		glm::mat4 scaleMat(
			dir.x, 0, 0, 0,
			0, dir.y, 0, 0,
			0, 0, dir.z, 0,
			0, 0, 0, 1);

		glm::mat4 translateMat(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			position.x, position.y, position.z, 1);

		glm::mat4 model = translateMat * scaleMat;

		m_pMaterial->use();
		m_pMaterial->set_model_matrix(model);
		m_pMaterial->get_shader_ptr()->setVec4(SS_COLOR_PROP_NAME, color);

		Renderer::drawLine(*m_vertexArray, m_size);
	}
	void Line::set_prj_mat(glm::mat4 prj)
	{
		m_pMaterial->use();
		m_pMaterial->set_view_projection_matrix(prj);
	}
	void Line::set_size(float size)
	{
		m_size = size;
	}
}
