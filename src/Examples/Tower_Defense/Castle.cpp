#include "Games/Tower_Defense/Castle.h"

#include "Games/Tower_Defense/HealthBar.h"

#include "EngineCore/Meshes/Cube.h"
#include "EngineCore/Components/Transform.h"

Castle::Castle(glm::vec3 initPos, const unsigned int hp, Cube* model, std::shared_ptr<RenderEngine::Material> pMaterial)
	: m_hp(hp)
	, m_model(std::move(model))
	, m_isDestroyed(false)
	, m_bar(new HealthBar(pMaterial, initPos + glm::vec3(0.f, 2.5f, 0.f), 25, 2, hp, glm::vec3(1.f), glm::vec3(1.f, 0.f, 0.f)))
{
	model->addComponent<Transform>(initPos);
}

void Castle::update()
{
	if (m_isDestroyed) return;
	m_model->update(0);
	m_bar->update();
}

void Castle::damage(const unsigned int damage_hp)
{
	m_hp -= damage_hp;
	m_bar->set_value(m_hp);
	if (m_hp <= 0)
	{
		m_isDestroyed = true;
	}
}

glm::vec3 Castle::get_pos()
{
	return m_model->getComponent<Transform>()->get_position();
}

bool Castle::isDestroyed()
{
	return m_isDestroyed;
}
