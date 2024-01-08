#pragma once

#include "Games/Tower_Defense/BaseEnemy.h"

#include "EngineCore/System/List.h"

#define P_MAGICIAN_VELOCITY 1.5
#define P_MAGICIAN_COOLDOWN 2.3
#define P_MAGICIAN_HP 30
#define P_MAGICIAN_DAMAGE 1.4
#define P_MAGICIAN_RADIUS_TREAT 4
#define P_MAGICIAN_DESCRIPTION "Evil magician, treats enemy around self"

namespace RenderEngine
{
	class Circle;
}

class MagicianEnemy : public BaseEnemy
{
public:
	MagicianEnemy(std::shared_ptr<GraphicsObject> model, std::shared_ptr<RenderEngine::Material> pMaterial,
		Castle* target, std::vector<Target> targets, glm::vec3 pos, std::shared_ptr<RenderEngine::Material> pMaterialLine, linked_list<BaseEnemy*>* list);
	~MagicianEnemy() = default;

	void on_render() override;
	void on_update(const double delta) override;

	std::string get_description() override { return P_MAGICIAN_DESCRIPTION; }
private:
	double m_dur_time_treat;
	double m_cur_time_treat;
	double m_cur_time_cir;
	double m_dur_time_cir;
	bool m_isActiveCircle;
	RenderEngine::Circle* m_health; 
	linked_list<BaseEnemy*>* m_list;
};