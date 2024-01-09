#include "EngineCore/Renderer3D/ParticleSystem.h"

#include "EngineCore/Renderer3D/Particle.h"
#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/System/ShadersSettings.h"
#include "EngineCore/System/SysFunc.h"
#include "EngineCore/Components/SpriteRenderer.h"

#define PARTICLE_SYSTEM_ADD_PERCENT 10

ParticleSystem::ParticleSystem(glm::vec3 pos, glm::vec3 particle_scale,
	glm::vec3 vel_move, glm::vec3 acc_move, glm::vec3 vel_rot, glm::vec3 acc_rot,
	int max_count_particles, double particle_live_time, float range_randomize, std::shared_ptr<RenderEngine::Material> pMaterial, bool isCyclic)
	: m_acc_move(acc_move)
	, m_vel_move(vel_move)
	, m_acc_rot(acc_rot)
	, m_vel_rot(vel_rot)
	, m_particle_scale(particle_scale)
	, m_pos(pos)
	, m_count_particles(0)
	, m_percent_spawn(10)
	, m_max_count_particles(max_count_particles)
	, m_particle_live_time(particle_live_time)
	, m_range(range_randomize)
	, m_pMaterial(std::move(pMaterial))
	, m_isDestroyed(false)
	, m_isCyclic(isCyclic)
{
}

ParticleSystem::~ParticleSystem()
{
	for (size_t i = 0; i < m_particles.size(); i++)
	{
		delete m_particles[i];
	}
	m_particles.clear();
}

void ParticleSystem::update(const double delta)
{
	if (m_isDestroyed) return;
	for (size_t i = 0; i < m_particles.size(); i++)
	{
		m_particles[i]->update(delta);
		if (m_isCyclic)
		{
			if (m_particles[i]->isDestroyed())
			{
				m_count_particles--;
				delete m_particles[i];
				m_particles.remove(i);
			}
		}
		else m_isDestroyed &= m_particles[i]->isDestroyed();
	}

	if (m_count_particles > m_max_count_particles) return;

	if (sysfunc::get_random(0, 100) < m_percent_spawn)
	{
		m_percent_spawn = 10; m_count_particles++;
		m_particles.push_back(new Particle(m_particle_live_time, m_pos, m_particle_scale, 
			glm::vec3(sysfunc::get_random(-m_range * 180, m_range * 180), sysfunc::get_random(-m_range * 180, m_range * 180), sysfunc::get_random(-m_range * 180, m_range * 180)),
			m_vel_move + glm::vec3(sysfunc::get_random(-m_range, m_range), sysfunc::get_random(0.f, m_range), sysfunc::get_random(-m_range, m_range)),
			m_acc_move,
			m_vel_rot + glm::vec3(sysfunc::get_random(-m_range, m_range), sysfunc::get_random(-m_range, m_range), sysfunc::get_random(-m_range, m_range)),
			m_acc_rot + glm::vec3(sysfunc::get_random(-m_range, m_range), sysfunc::get_random(-m_range, m_range), sysfunc::get_random(-m_range, m_range)), m_pMaterial));
	}
	else
	{
		m_percent_spawn += PARTICLE_SYSTEM_ADD_PERCENT;
	}	
}

void ParticleSystem::render(glm::mat4 prj)
{
	if (m_particles.size() == 0) return;
	std::shared_ptr<RenderEngine::ShaderProgram> shader = m_particles[0]->getComponent<SpriteRenderer>()->get_material_ptr()->get_shader_ptr();
	shader->use();
	shader->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, prj);
	for (size_t i = 0; i < m_particles.size(); i++)
	{
		m_particles[i]->render();
	}
}

