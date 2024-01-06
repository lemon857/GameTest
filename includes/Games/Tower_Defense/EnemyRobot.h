#pragma once

#include "Games/Tower_Defense/BaseEnemy.h"

#define P_ROBOT_VELOCITY 1
#define P_ROBOT_COOLDOWN 3.4
#define P_ROBOT_HP 50
#define P_ROBOT_DAMAGE 2.3
#define P_ROBOT_DESCRIPTION "So heavy machine"

class RobotEnemy : public BaseEnemy
{
public:
	RobotEnemy(ObjModel* model, Castle* target, std::vector<Target> targets, glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterial);
	~RobotEnemy() = default;

	std::string get_description() override { return P_ROBOT_DESCRIPTION; }
private:

};