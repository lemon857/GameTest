#pragma once

#include "EngineCore/IGameObject.h"

#include <glm/vec3.hpp>
#include <memory>
#include <string>

class BaseEnemy;
class ObjModel;
class Sound;

namespace RenderEngine
{
	class Material;
	class Line;
}

struct GraphicsObject;

class BaseTower : public IGameObject
{
public:
	BaseTower(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial, BaseEnemy* target, glm::vec3 pos, double cooldown, unsigned int damage, RenderEngine::Line* line);
	~BaseTower();

	void update(const double delta) override;

	void render() override;

	void set_target(BaseEnemy* target);

	glm::vec3 get_pos();

	BaseEnemy* get_target();

private:
	BaseEnemy* m_target_BaseEnemy;

	double m_cur_time;

	double m_cool_down;

	unsigned int m_damage;

	RenderEngine::Line* m_line;

	std::unique_ptr<Sound> m_sound;
};