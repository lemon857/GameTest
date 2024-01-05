#pragma once

#include "Games/Tower_Defense/BaseEnemy.h"

#define P_MONKEY_VELOCITY 1.3
#define P_MONKEY_COOLDOWN 1.4
#define P_MONKEY_HP 50
#define P_MONKEY_DAMAGE 1

class MonkeyEnemy : public BaseEnemy
{
public:
	MonkeyEnemy(ObjModel* model, Castle* target, std::vector<Target> targets, glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterial);
	~MonkeyEnemy() = default; 

private:

};