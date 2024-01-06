#pragma once

#include "EngineCore/IGameObject.h"

#include <glm/vec3.hpp>

#include <memory>
#include <string>

class HealthBar;

struct GraphicsObject;

namespace RenderEngine
{
	class Material;
}

class Castle : public IGameObject
{
public:
	Castle(glm::vec3 initPos, const unsigned int hp, std::shared_ptr<GraphicsObject> obj,
		std::shared_ptr<RenderEngine::Material> pMaterial, std::shared_ptr<RenderEngine::Material> pMaterialBar, glm::vec3 colorBar = glm::vec3(1.f, 0.f, 0.f));

	~Castle() = default;

	void update(const double delta) override;

	void render() override;

	void damage(const double damage_hp);
		
	glm::vec3 get_pos();

	bool isDestroyed();

	double get_hp();
	double get_max_hp();

private:
	HealthBar* m_bar;

	double m_hp;
	double m_max_hp;

	bool m_isDestroyed;
};
