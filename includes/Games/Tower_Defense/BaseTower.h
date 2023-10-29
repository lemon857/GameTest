#pragma once

#include "EngineCore/IGameObject.h"

#include <glm/vec3.hpp>
#include <memory>
#include <string>

class Enemy;
class ObjModel;

namespace RenderEngine
{
	class Material;
	class Line;
}

class BaseTower : public IGameObject
{
public:
	BaseTower(std::string objPath, std::shared_ptr<RenderEngine::Material> pMaterial, Enemy* target, glm::vec3 pos, double cooldown, RenderEngine::Line* line);
	~BaseTower();

	void update(const double delta) override;

	void set_target(Enemy* target);

	glm::vec3 get_pos();

	Enemy* get_target();

private:
	Enemy* m_target_enemy;

	double m_cur_time;

	double m_cool_down;

	RenderEngine::Line* m_line;
};