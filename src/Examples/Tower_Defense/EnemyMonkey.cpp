#include "Games/Tower_Defense/EnemyMonkey.h"

#include "Games/Tower_Defense/DamageTable.h"

MonkeyEnemy::MonkeyEnemy(std::shared_ptr<GraphicsObject> model, std::shared_ptr<RenderEngine::Material> pMaterial,
	Castle* target, std::vector<Target> targets, glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterialLine)
	: BaseEnemy(std::move(model), std::move(pMaterial), std::move(target), targets, pos, P_MONKEY_COOLDOWN, P_MONKEY_VELOCITY, P_MONKEY_HP, P_MONKEY_DAMAGE, std::move(pMaterialLine),
		glm::vec3(1.f, 0.4f, 0.f))
{
	m_type_armor = TypeArmor::Light;
	m_reward = 4;
}
