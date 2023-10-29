#include "EngineCore/Meshes/Grid.h"

#include "EngineCore/Renderer/Line.h"
#include "EngineCore/Renderer/Material.h"

static unsigned int s_current_grids_ID = 0;

Grid::Grid(glm::vec3 pos, glm::vec2 size, unsigned int count_cols, unsigned int count_rows, glm::vec3 color, std::shared_ptr<RenderEngine::Material> pMaterial)
	: IGameObject("Grid" + std::to_string(s_current_grids_ID++))
	, m_count_firsts(count_cols)
	, m_color(color)
{
	size *= 2.f;
	float size_x = size.x * (float)count_cols;
	float size_z = size.y * (float)count_rows;

	glm::vec3 cur_pos = pos + glm::vec3(size_x / 2.f, 0.f, -size_z / 2.f);

	m_first_dir = glm::vec3(0.f, 0.f, size_z);
	m_last_dir = glm::vec3(size_x, 0.f, 0.f);

	for (unsigned int i = 0; i < count_cols; i++)
	{
		m_starts_pos.push_back(cur_pos);
		cur_pos += glm::vec3(-size.x, 0.f, 0.f);
	}
	m_starts_pos.push_back(cur_pos);
	for (unsigned int j = 0; j < count_rows + 1; j++)
	{
		m_starts_pos.push_back(cur_pos);
		cur_pos += glm::vec3(0.f, 0.f, size.y);
	}
	m_line = new RenderEngine::Line(pMaterial);
}

Grid::~Grid()
{
	delete m_line;
}

void Grid::update(const double delta)
{
	for (size_t i = 0; i < m_starts_pos.size(); i++)
	{
		m_line->render(m_starts_pos[i],  i <= m_count_firsts ? m_first_dir : m_last_dir, m_color);
	}
}