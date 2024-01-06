#include "Games/Tower_Defense/EnemyProfessor.h"

#include "Games/Tower_Defense/DamageTable.h"

ProfessorEnemy::ProfessorEnemy(ObjModel* model, Castle* target, std::vector<Target> targets,
	glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterial) 
	: BaseEnemy(std::move(model), std::move(target), targets, pos, P_PROFESSOR_COOLDOWN, P_PROFESSOR_VELOCITY, P_PROFESSOR_HP, P_PROFESSOR_DAMAGE, pMaterial,
		glm::vec3(0.5f, 0.1f, 0.8f))
{
	m_type_armor = TypeArmor::Chaotic;
	m_reward = 7;
}
