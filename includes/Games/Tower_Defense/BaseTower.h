#pragma once

#include "EngineCore/IGameObject.h"

#include <glm/vec3.hpp>
#include <memory>
#include <string>

class BaseEnemy;
class ObjModel;

namespace RenderEngine
{
	class Material;
	class Line;
}

class BaseTower : public IGameObject
{
public:
	BaseTower(std::string objPath, std::shared_ptr<RenderEngine::Material> pMaterial, BaseEnemy* target, glm::vec3 pos, double cooldown, unsigned int damage, RenderEngine::Line* line);
	~BaseTower();

	void update(const double delta) override;

	void set_target(BaseEnemy* target);

	glm::vec3 get_pos();

	BaseEnemy* get_target();

private:
	BaseEnemy* m_target_BaseEnemy;

	double m_cur_time;

	double m_cool_down;

	unsigned int m_damage;

	RenderEngine::Line* m_line;
};