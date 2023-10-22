#pragma once

#include "EngineCore/IGameObject.h"

#include <glm/vec3.hpp>

#include <memory>
#include <string>

class HealthBar;

namespace RenderEngine
{
	class Material;
}

class Castle : public IGameObject
{
public:
	Castle(glm::vec3 initPos, const unsigned int hp, std::string objPath, std::shared_ptr<RenderEngine::Material> pMaterial, std::shared_ptr<RenderEngine::Material> pMaterialBar);

	~Castle() = default;

	void update(const double delta) override;

	void damage(const unsigned int damage_hp);
		
	glm::vec3 get_pos();

	bool isDestroyed();

private:
	HealthBar* m_bar;

	unsigned int m_hp;

	bool m_isDestroyed;
};
