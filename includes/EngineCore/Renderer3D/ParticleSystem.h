#pragma once

#include "EngineCore/System/List.h"

#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace RenderEngine
{
	class Material;
}


class Particle;

class ParticleSystem
{
public:
	ParticleSystem(glm::vec3 pos, glm::vec3 particle_scale,
		glm::vec3 vel_move, glm::vec3 acc_move, glm::vec3 vel_rot, glm::vec3 acc_rot,
		int max_count_particles, double particle_live_time, float range_randomize, std::shared_ptr<RenderEngine::Material> pMaterial, bool isCyclic = false);
	~ParticleSystem();

	void update(const double delta);

	void render(glm::mat4 prj);
	
	bool is_destroyed() { return m_isDestroyed; }

	void stop() { m_isCyclic = false; }
private:
	glm::vec3 m_pos;
	glm::vec3 m_particle_scale;

	glm::vec3 m_vel_rot;
	glm::vec3 m_acc_rot;
	glm::vec3 m_vel_move;
	glm::vec3 m_acc_move;

	double m_particle_live_time;

	double m_percent_spawn;

	int m_count_particles;
	int m_max_count_particles;

	float m_range;

	bool m_isDestroyed;
	bool m_isCyclic;

	linked_list<Particle*> m_particles;
	std::shared_ptr<RenderEngine::Material> m_pMaterial;
};