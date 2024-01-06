#pragma once

#include "Games/Tower_Defense/BaseEnemy.h"

#define P_MAGICIAN_VELOCITY 2
#define P_MAGICIAN_COOLDOWN 2.3
#define P_MAGICIAN_HP 30
#define P_MAGICIAN_DAMAGE 1.4
#define P_MAGICIAN_DESCRIPTION "Evil magician"

class MagicianEnemy : public BaseEnemy
{
public:
	MagicianEnemy(ObjModel* model, Castle* target, std::vector<Target> targets, glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterial);
	~MagicianEnemy() = default;

	std::string get_description() override { return P_MAGICIAN_DESCRIPTION; }
private:

};