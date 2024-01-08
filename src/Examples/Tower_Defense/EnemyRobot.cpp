#include "Games/Tower_Defense/EnemyRobot.h"

#include "Games/Tower_Defense/DamageTable.h"

RobotEnemy::RobotEnemy(std::shared_ptr<GraphicsObject> model, std::shared_ptr<RenderEngine::Material> pMaterial,
	Castle* target, std::vector<Target> targets, glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterialLine)
	: BaseEnemy(std::move(model), std::move(pMaterial), std::move(target), targets, pos, P_ROBOT_COOLDOWN, P_ROBOT_VELOCITY, P_ROBOT_HP, P_ROBOT_DAMAGE, std::move(pMaterialLine),
		glm::vec3(0.33f, 0.33f, 0.33f))
{
	m_type_armor = TypeArmor::Heavy;
	m_reward = 10;
}
