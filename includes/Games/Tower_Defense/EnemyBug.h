#pragma once

#include "Games/Tower_Defense/BaseEnemy.h"

#define P_BUG_VELOCITY 2
#define P_BUG_COOLDOWN 1
#define P_BUG_HP 25
#define P_BUG_DAMAGE 0.5
#define P_BUG_DESCRIPTION "Faster disgusting bug"

class BugEnemy : public BaseEnemy
{
public:
	BugEnemy(ObjModel* model, Castle* target, std::vector<Target> targets, glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterial);
	~BugEnemy() = default;

	std::string get_description() override { return P_BUG_DESCRIPTION; }
private:

};