#include "Games/Tower_Defense/EnemySpider.h"

#include "Games/Tower_Defense/DamageTable.h"

SpiderEnemy::SpiderEnemy(ObjModel* model, Castle* target, std::vector<Target> targets,
	glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterial)
	: BaseEnemy(std::move(model), std::move(target), targets, pos, P_SPIDER_COOLDOWN, P_SPIDER_VELOCITY, P_SPIDER_HP, P_SPIDER_DAMAGE, pMaterial,
		glm::vec3(0.f, 0.8f, 0.4f))
{
	m_type_armor = TypeArmor::Null;
	m_reward = 3;
}
