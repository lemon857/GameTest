#include "Games/Tower_Defense/EnemyMagician.h"

#include "Games/Tower_Defense/DamageTable.h"
#include "EngineCore/Meshes/ObjModel.h"
#include "EngineCore/Components/Transform.h"

MagicianEnemy::MagicianEnemy(ObjModel* model, Castle* target, std::vector<Target> targets, glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterial)
	: BaseEnemy(std::move(model), std::move(target), targets, pos, P_MAGICIAN_COOLDOWN, P_MAGICIAN_VELOCITY, P_MAGICIAN_HP, P_MAGICIAN_DAMAGE, std::move(pMaterial),
		glm::vec3(1.f, 0.4f, 0.f))
{
	m_type_armor = TypeArmor::Magic;
	m_reward = 6;
	m_model->getComponent<Transform>()->set_scale(glm::vec3(0.5f));
}
