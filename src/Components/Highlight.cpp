#include "EngineCore/Components/Highlight.h"

#include "EngineCore/IGameObject.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Renderer/Line.h"
#include "EngineCore/Components/Transform.h"

#include <iostream>

Highlight::Highlight(std::shared_ptr<RenderEngine::Material> material, bool isActive, glm::vec3 color)
	: IComponent()
	, m_line(std::make_unique<RenderEngine::Line>(material))
	, m_color(color)
	, m_isActive(isActive)
{
}

void Highlight::update(const double delta)
{
	if (!m_isActive) return;
	Transform* transform = m_targetObj->getComponent<Transform>();
	glm::vec3 pos;
	glm::vec3 scale;
	if (transform == nullptr)
	{
		scale = glm::vec3(1.f);;
		pos = glm::vec3(0.f);;
	}
	else
	{
		scale = transform->get_scale();
		pos = transform->get_position();
	}

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

void Highlight::set_active(bool isActive)
{
	m_isActive = isActive;
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
}