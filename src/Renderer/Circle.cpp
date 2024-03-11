#include "EngineCore/Renderer/Circle.h"

#include "EngineCore/System/ShadersSettings.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexArray.h"

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine
{
	Circle::Circle(Material* pMaterial, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot, glm::vec4 color, float radius, int numSegments, float size)
		: m_pos(pos)
		, m_rot(rot)
		, m_scale(scale)
		, m_pMaterial(std::move(pMaterial))
		, m_size(size)
		, m_numSegments(numSegments)
		, m_radius(radius)
		, m_color(color)
	{
		std::vector<GLfloat> poss;
		for (int ii = 0; ii < m_numSegments; ii++) {
			float theta = 2.0f * 3.1415926f * float(ii) / float(m_numSegments);//get the current angle 
			float x = radius * cosf(theta);//calculate the x component 
			float y = radius * sinf(theta);//calculate the y component 
			poss.push_back(x);
			poss.push_back(0.f);
			poss.push_back(y);
		}

		m_vertexCoordsBuffer = new VertexBuffer();
		m_vertexArray = new VertexArray();

		m_vertexCoordsBuffer->init(poss.data(), 3 * m_numSegments * sizeof(GLfloat), false);
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		m_vertexArray->addBuffer(*m_vertexCoordsBuffer, vertexCoordsLayout);

		m_vertexArray->unbind();
	}
	Circle::~Circle()
	{
		m_vertexCoordsBuffer->unbind();
		m_vertexArray->unbind();
		delete m_vertexCoordsBuffer;
		delete m_vertexArray;
	}
	void Circle::render()
	{
		m_pMaterial->use();

		glm::mat4 scaleMat(
			m_scale.x, 0, 0, 0,
			0, m_scale.y, 0, 0,
			0, 0, m_scale.z, 0,
			0, 0, 0, 1);

		glm::mat4 translateMat(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			m_pos.x, m_pos.y, m_pos.z, 1);

		float xRotRadians = glm::radians(m_rot[0]);
		float yRotRadians = glm::radians(m_rot[1]);
		float zRotRadians = glm::radians(m_rot[2]);

		glm::mat4 rotateXmat(
			1, 0, 0, 0,
			0, cos(xRotRadians), -sin(xRotRadians), 0,
			0, sin(xRotRadians), cos(xRotRadians), 0,
			0, 0, 0, 1);

		glm::mat4 rotateYmat(
			cos(yRotRadians), 0, -sin(yRotRadians), 0,
			0, 1, 0, 0,
			sin(yRotRadians), 0, cos(yRotRadians), 0,
			0, 0, 0, 1);

		glm::mat4 rotateZmat(
			cos(zRotRadians), -sin(zRotRadians), 0, 0,
			sin(zRotRadians), cos(zRotRadians), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		glm::mat4 model = translateMat * rotateXmat * rotateYmat * rotateZmat * scaleMat;

		m_pMaterial->set_model_matrix(model);
		m_pMaterial->get_shader_ptr()->setVec4(SS_COLOR_PROP_NAME, m_color);

		Renderer::drawLine(*m_vertexArray, m_size, m_numSegments);
	}
	void Circle::update(float radius, int numSegments)
	{
		if (m_radius == radius && m_numSegments == numSegments) return;
		m_radius = radius;
		m_numSegments = numSegments;
		std::vector<GLfloat> poss;
		for (int ii = 0; ii < m_numSegments; ii++) {
			float theta = 2.0f * 3.1415926f * float(ii) / float(m_numSegments);//get the current angle 
			float x = m_radius * cosf(theta);//calculate the x component 
			float y = m_radius * sinf(theta);//calculate the y component 
			poss.push_back(x);
			poss.push_back(0.f);
			poss.push_back(y);
		}

		m_vertexCoordsBuffer->update(poss.data(), 3 * m_numSegments * sizeof(GLfloat));

		m_vertexArray->unbind();
	}
	void Circle::set_pos(glm::vec3 pos)
	{
		m_pos = pos;
	}
	void Circle::set_rot(glm::vec3 rot)
	{
		m_rot = rot;
	}
	void Circle::set_rad(float radius)
	{
		if (m_radius == radius) return;
		m_radius = radius;
		std::vector<GLfloat> poss;
		for (int ii = 0; ii < m_numSegments; ii++) {
			float theta = 2.0f * 3.1415926f * float(ii) / float(m_numSegments);//get the current angle 
			float x = m_radius * cosf(theta);//calculate the x component 
			float y = m_radius * sinf(theta);//calculate the y component 
			poss.push_back(x);
			poss.push_back(0.f);
			poss.push_back(y);
		}

		m_vertexCoordsBuffer->update(poss.data(), 3 * m_numSegments * sizeof(GLfloat));

		m_vertexArray->unbind();
	}
	void Circle::add_rot(glm::vec3 rot)
	{
		m_rot += rot;
	}
}