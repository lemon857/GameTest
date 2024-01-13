#include "EngineCore/GUI/Bar.h"

#include "EngineCore/Renderer/Line.h"
#include "EngineCore/Renderer/Material.h"

GUI::Bar::Bar(std::shared_ptr<RenderEngine::Material> pMaterial, 
	glm::vec2 pos_p, glm::vec2 scale_p, 
	float size_line, double max_value,
	glm::vec3 background_color, glm::vec3 foreground_color, std::string name)
	: GUI_element(name, std::move(pMaterial))
	, m_max_value(max_value)
	, m_value(0)
	, m_background_color(background_color)
	, m_foreground_color(foreground_color)
{
	m_position_p = pos_p;
	m_scale_p = scale_p;

	m_background_line = new RenderEngine::Line(m_pMaterial, 1);
	m_foreground_line = new RenderEngine::Line(m_pMaterial, 1);
}

GUI::Bar::~Bar()
{
	delete m_foreground_line;
	delete m_background_line;
}

void GUI::Bar::on_render_prj(glm::mat4& prj)
{
	if (!m_isActive) return;

	m_pMaterial->use();
	m_pMaterial->set_view_projection_matrix(prj);

	m_background_line->render_from_to(glm::vec3(m_position.x + 2 * m_scale.x * m_value / m_max_value - m_scale.x, m_position.y, m_layer),
		glm::vec3(m_position.x + m_scale.x, m_position.y, m_layer), m_background_color);
	m_foreground_line->render_from_to(glm::vec3(m_position.x - m_scale.x, m_position.y, m_layer),
		glm::vec3(m_position.x + 2 * m_scale.x * m_value / m_max_value - m_scale.x, m_position.y, m_layer), m_foreground_color);
}

void GUI::Bar::add_value(const double value)
{
	m_value += value;
}

void GUI::Bar::add_max_value(const double value)
{
	m_max_value += value;
}

void GUI::Bar::set_value(const double value)
{
	m_value = value;
}

void GUI::Bar::set_max_value(const double max_val)
{
	m_max_value = max_val;
}

bool GUI::Bar::is_full()
{
	return m_value >= m_max_value;
}

void GUI::Bar::set_scale(glm::vec2 scale)
{
	m_scale = scale;
	m_foreground_line->set_size(m_scale.y);
	m_background_line->set_size(m_scale.y);		
}
