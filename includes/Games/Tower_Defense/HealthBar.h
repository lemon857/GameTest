#pragma once

#include <memory>

#include <glm/vec3.hpp>

namespace RenderEngine 
{
	class Line;
	class Material;
}

class HealthBar
{
public:
	HealthBar(std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec3 pos, float size, float lenght,
		double max_value, glm::vec3 background_color, glm::vec3 foreground_color);

	~HealthBar();

	void update();

	void set_value(const double value);

	void set_max_value(const double max_val);

	void set_pos(glm::vec3 pos);

	void set_fore_color(glm::vec3 col);
	void set_back_color(glm::vec3 col);

private:
	// Линия заднего фона бара
	RenderEngine::Line* m_background_line;
	// Линия отображения информации
	RenderEngine::Line* m_foreground_line;
	glm::vec3 m_pos;
	double m_max_value;
	double m_value;
	float m_lenght;
	glm::vec3 m_background_color;
	glm::vec3 m_foreground_color;
};
