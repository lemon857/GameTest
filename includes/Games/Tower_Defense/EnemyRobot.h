#pragma once

#include "Games/Tower_Defense/BaseEnemy.h"

#define P_ROBOT_VELOCITY 1
#define P_ROBOT_COOLDOWN 3.4
#define P_ROBOT_HP 50
#define P_ROBOT_DAMAGE 2.3

class RobotEnemy : public BaseEnemy
{
public:
	RobotEnemy(std::shared_ptr<GraphicsObject> model, std::shared_ptr<RenderEngine::Material> pMaterial,
		Castle* target, std::vector<Target> targets, glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterialLine);
	~RobotEnemy() = default;

private:

};