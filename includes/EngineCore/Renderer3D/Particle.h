#pragma once

#include "EngineCore/IGameObject.h"

#include <memory>
#include <glm/vec3.hpp>

namespace RenderEngine
{
	class Material;
}

class Particle : public IGameObject
{
public:
	Particle(double live_time, glm::vec3 startPos, glm::vec3 startScale, glm::vec3 startRot,
		glm::vec3 vel_m, glm::vec3 acc_m, glm::vec3 vel_r, glm::vec3 m_acc_r,
		RenderEngine::Material* pMaterial);

	void update(const double delta) override;

	void render() override;

	bool isDestroyed() { return m_isDestroyed; }
private:
	// Velocity move
	glm::vec3 m_vel_m;
	// Acceleration move
	glm::vec3 m_acc_m;

	// Velocity rotation
	glm::vec3 m_vel_r;
	// Acceleration rotation
	glm::vec3 m_acc_r;

	double m_live_time;
	double m_cur_time;

	bool m_isDestroyed;
};