#pragma once

#include <glm/vec3.hpp>

#include <memory>

class Cube;
class HealthBar;

namespace RenderEngine
{
	class Material;
}

class Castle
{
public:
	Castle(glm::vec3 initPos, const unsigned int hp, Cube* model, std::shared_ptr<RenderEngine::Material> pMaterial);

	~Castle() = default;

	void update();

	void damage(const unsigned int damage_hp);
		
	glm::vec3 get_pos();

	bool isDestroyed();

private:
	Cube* m_model;

	HealthBar* m_bar;

	unsigned int m_hp;

	bool m_isDestroyed;
};
