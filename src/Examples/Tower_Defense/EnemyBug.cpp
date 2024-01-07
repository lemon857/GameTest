#include "Games/Tower_Defense/EnemyBug.h"

#include "Games/Tower_Defense/DamageTable.h"

BugEnemy::BugEnemy(ObjModel* model, Castle* target, std::vector<Target> targets,
	glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterial)
	: BaseEnemy(std::move(model), std::move(target), targets, pos, P_BUG_COOLDOWN, P_BUG_VELOCITY, P_BUG_HP, P_BUG_DAMAGE, pMaterial,
		glm::vec3(0.f, 0.8f, 0.4f))
{
	m_type_armor = TypeArmor::Null;
	m_reward = 3;
}
