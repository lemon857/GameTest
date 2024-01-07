#include "Games/Tower_Defense/HealthBar.h"

#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Renderer/Line.h"

#include "EngineCore/System/ShadersSettings.h"

HealthBar::HealthBar(std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec3 pos, float size, float lenght, double max_value,
	glm::vec3 background_color, glm::vec3 foreground_color)
	: m_max_value(max_value)
	, m_background_color(background_color)
	, m_foreground_color(foreground_color)
	, m_background_line(new RenderEngine::Line(pMaterial, size))
	, m_foreground_line(new RenderEngine::Line(pMaterial, size - 1))
	, m_pos(pos)
	, m_lenght(lenght)
	, m_value(max_value)
{
}

HealthBar::~HealthBar()
{
	delete m_background_line;
	delete m_foreground_line;
}

void HealthBar::render()
{
	m_background_line->render_from_to(glm::vec3(m_pos.x, m_pos.y, m_pos.z + 2 * m_lenght * m_value / m_max_value - m_lenght),
		glm::vec3(m_pos.x, m_pos.y, m_pos.z + m_lenght), m_background_color);
	m_foreground_line->render_from_to(glm::vec3(m_pos.x, m_pos.y, m_pos.z - m_lenght),
			glm::vec3(m_pos.x, m_pos.y, m_pos.z + 2 * m_lenght * m_value / m_max_value - m_lenght), m_foreground_color);
}

void HealthBar::set_value(const double value)
{
	if (value >= 0 && value <= m_max_value) m_value = value;
}

void HealthBar::set_max_value(const double max_val)
{
	m_max_value = max_val;
}

void HealthBar::set_pos(glm::vec3 pos)
{
	m_pos = pos;
}

void HealthBar::set_fore_color(glm::vec3 col)
{
	m_foreground_color = col;
}

void HealthBar::set_back_color(glm::vec3 col)
{
	m_background_color = col;
}
