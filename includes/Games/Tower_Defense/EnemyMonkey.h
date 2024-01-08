#pragma once

#include "Games/Tower_Defense/BaseEnemy.h"

#define P_MONKEY_VELOCITY 1.3
#define P_MONKEY_COOLDOWN 1.4
#define P_MONKEY_HP 35
#define P_MONKEY_DAMAGE 1
#define P_MONKEY_DESCRIPTION "Funny monkey"

class MonkeyEnemy : public BaseEnemy
{
public:
	MonkeyEnemy(std::shared_ptr<GraphicsObject> model, std::shared_ptr<RenderEngine::Material> pMaterial, Castle* target, std::vector<Target> targets, glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterialLine);
	~MonkeyEnemy() = default; 

	std::string get_description() override { return P_MONKEY_DESCRIPTION; }
private:

};