#include "EngineCore/Renderer3D/Particle.h"

#include "EngineCore/Components/Transform.h"
#include "EngineCore/Components/SpriteRenderer.h"

#define PARTICLE_MULTIPLY_VELOCITY 0.001f

Particle::Particle(double live_time, glm::vec3 startPos, glm::vec3 startScale, glm::vec3 startRot, glm::vec3 vel_m, glm::vec3 acc_m, glm::vec3 vel_r, glm::vec3 acc_r, std::shared_ptr<RenderEngine::Material> pMaterial)
	: m_isDestroyed(false)
	, m_cur_time(0)
	, m_live_time(live_time)
	, m_vel_m(vel_m)
	, m_vel_r(vel_r)
	, m_acc_m(acc_m * PARTICLE_MULTIPLY_VELOCITY)
	, m_acc_r(acc_r * PARTICLE_MULTIPLY_VELOCITY)
{
	addComponent<Transform>(startPos, startScale, startRot);
	addComponent<SpriteRenderer>(std::move(pMaterial));
}

Particle::~Particle()
{
}

void Particle::update(const double delta)
{
	if (m_isDestroyed) return;
	m_vel_m += m_acc_m * (float)delta;
	m_vel_r += m_acc_r * (float)delta;

	getComponent<Transform>()->add_position(m_vel_m);
	getComponent<Transform>()->add_rotation(m_vel_r);

	m_cur_time += delta;
	if (m_live_time <= m_cur_time)
	{
		m_isDestroyed = true;
	}

	updateComponents(delta);
}

void Particle::render()
{
	if (m_isDestroyed) return;
	renderComponents();
}
