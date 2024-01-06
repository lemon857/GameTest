#pragma once

#include "Games/Tower_Defense/BaseEnemy.h"

#define P_SPIDER_VELOCITY 2
#define P_SPIDER_COOLDOWN 1
#define P_SPIDER_HP 25
#define P_SPIDER_DAMAGE 0.5
#define P_SPIDER_DESCRIPTION "Faster spider"

class SpiderEnemy : public BaseEnemy
{
public:
	SpiderEnemy(ObjModel* model, Castle* target, std::vector<Target> targets, glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterial);
	~SpiderEnemy() = default;

	std::string get_description() override { return P_SPIDER_DESCRIPTION; }
private:

};