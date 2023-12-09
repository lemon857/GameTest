#include "EngineCore/Components/Highlight.h"

#include "EngineCore/IGameObject.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Renderer/Line.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Components/MeshRenderer.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer3D/GraphicsObject.h"
#include "EngineCore/System/ShadersSettings.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define ADDITION_SCALE_HIGHLIGHT 1.1f

Highlight::Highlight(std::shared_ptr<RenderEngine::Material> material, bool isActive, bool mode, glm::vec3 color)
	: IComponent()
	, m_line(std::make_unique<RenderEngine::Line>(material))
	, m_color(color)
	, m_isActive(isActive)
	, m_pMaterial(std::move(material))
	, m_mode(mode)
{
}

void Highlight::update(const double delta)
{
	if (!m_isActive) return;
	Transform* transform = m_targetObj->getComponent<Transform>();
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	if (transform == nullptr)
	{
		scale = glm::vec3(m_mode ? ADDITION_SCALE_HIGHLIGHT : 1.f);
		pos = glm::vec3(0.f);
		rot = glm::vec3(0.f);
	}
	else
	{
		scale = transform->get_scale() * (m_mode ? ADDITION_SCALE_HIGHLIGHT : 1.f);
		pos = transform->get_position();
		rot = transform->get_rotation();
	}
	if (m_mode)
	{
		MeshRenderer* renderer = m_targetObj->getComponent<MeshRenderer>();

		if (renderer == nullptr) return;

		std::shared_ptr<GraphicsObject> obj = renderer->get_object();

		if (obj == nullptr) return;

		glm::mat4 scaleMat(
			scale[0], 0, 0, 0,
			0, scale[1], 0, 0,
			0, 0, scale[2], 0,
			0, 0, 0, 1);

		glm::mat4 translateMat(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			pos[0], pos[1], pos[2], 1);

		float xRotRadians = glm::radians(rot[0]);
		float yRotRadians = glm::radians(rot[1]);
		float zRotRadians = glm::radians(rot[2]);

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

		m_pMaterial->use();
		m_pMaterial->set_model_matrix(model);

		RenderEngine::Renderer::setDepthTest(false);
		RenderEngine::Renderer::drawTriangles(*obj->vertex_array, *obj->index_buffer);
		RenderEngine::Renderer::setDepthTest(true);
	}
	else
	{
		m_line->render(pos - scale, glm::vec3(2 * scale.x, 0.f, 0.f), m_color);
		m_line->render(pos - scale, glm::vec3(0.f, 2 * scale.y, 0.f), m_color);
		m_line->render(pos - scale, glm::vec3(0.f, 0.f, 2 * scale.z), m_color);

		m_line->render(pos + scale, glm::vec3(-2 * scale.x, 0.f, 0.f), m_color);
		m_line->render(pos + scale, glm::vec3(0.f, -2 * scale.y, 0.f), m_color);
		m_line->render(pos + scale, glm::vec3(0.f, 0.f, -2 * scale.z), m_color);

		m_line->render(pos + glm::vec3(scale.x, -scale.y, scale.z), glm::vec3(-2 * scale.x, 0.f, 0.f), m_color);
		m_line->render(pos + glm::vec3(scale.x, -scale.y, scale.z), glm::vec3(0.f, 0.f, -2 * scale.z), m_color);

		m_line->render(pos + glm::vec3(scale.x, scale.y, -scale.z), glm::vec3(-2 * scale.x, 0.f, 0.f), m_color);
		m_line->render(pos + glm::vec3(scale.x, scale.y, -scale.z), glm::vec3(0.f, -2 * scale.y, 0.f), m_color);

		m_line->render(pos + glm::vec3(-scale.x, scale.y, scale.z), glm::vec3(0.f, 0.f, -2 * scale.z), m_color);
		m_line->render(pos + glm::vec3(-scale.x, scale.y, scale.z), glm::vec3(0.f, -2 * scale.y, 0.f), m_color);
	}
}

void Highlight::set_active(bool isActive)
{
	m_isActive = isActive;
}

void Highlight::set_mode(bool mode)
{
	m_mode = mode;
}

bool Highlight::get_active()
{
	return m_isActive;
}

glm::vec3 Highlight::get_color()
{
	return m_color;
}

void Highlight::set_color(glm::vec3 color)
{
	m_color = color;

	m_pMaterial->get_data<float>(SS_COLOR_PROP_NAME)[1] = m_color.y;
	m_pMaterial->get_data<float>(SS_COLOR_PROP_NAME)[2] = m_color.z;
	m_pMaterial->get_data<float>(SS_COLOR_PROP_NAME)[0] = m_color.x;
}