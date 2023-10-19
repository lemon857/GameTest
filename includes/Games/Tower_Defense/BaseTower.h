#pragma once

#define MIN_DISTANCE_TO_ENEMY 7.f

#include <glm/vec3.hpp>

class Enemy;
class ObjModel;

namespace RenderEngine
{
	class Material;
	class Line;
}

class BaseTower
{
public:
	BaseTower(ObjModel* model, Enemy* target, glm::vec3 pos, double cooldown, RenderEngine::Line* line);
	~BaseTower();

	void update(const double delta);

	void set_target(Enemy* target);

private:
	Enemy* m_target_enemy;
	ObjModel* m_model;

	double m_cur_time;

	double m_cool_down;

	RenderEngine::Line* m_line;
};