#pragma once

#include "EngineCore/IGameObject.h"

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <memory>

namespace RenderEngine
{
	class Line;
	class Material;
}

class Grid : public IGameObject
{
public:
	// center pos, size part, count cols to left and right from center, count rows to up and down from center 
	Grid(glm::vec3 pos, glm::vec2 size, unsigned int count_cols, unsigned int count_rows, glm::vec3 color, std::shared_ptr<RenderEngine::Material> pMaterial);
	~Grid();

	void update(const double delta) override;

private:
	std::vector<glm::vec3> m_starts_pos;
	glm::vec3 m_first_dir;
	glm::vec3 m_last_dir;
	glm::vec3 m_color;
	unsigned int m_count_firsts;

	RenderEngine::Line* m_line;
};