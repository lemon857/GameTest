#pragma once

#include "Games/Tower_Defense/BaseEnemy.h"

#define P_PROFESSOR_VELOCITY 1.2
#define P_PROFESSOR_COOLDOWN 1.6
#define P_PROFESSOR_HP 28
#define P_PROFESSOR_DAMAGE 1.5

class ProfessorEnemy : public BaseEnemy
{
public:
	ProfessorEnemy(std::shared_ptr<GraphicsObject> model, std::shared_ptr<RenderEngine::Material> pMaterial,
		Castle* target, std::vector<Target> targets, glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterialLine);
	~ProfessorEnemy() = default;

private:

};