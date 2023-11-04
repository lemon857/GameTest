#pragma once 

#include "EngineCore/Application.h"

#include "EngineCore/Renderer/Camera.h"
#include "EngineCore/Renderer/Line.h"
#include "EngineCore/Resources/Scene.h"

#include <vector>
#include <glm/vec3.hpp>
	
class GameApp : public Application
{
public:
	GameApp();
	~GameApp();

	bool init() override;
	void on_key_update(const double delta) override;
	void on_update(const double delta) override;
	bool init_events() override;
private:
	Scene m_scene;

	Camera* m_cam;

	RenderEngine::Line* m_grid_line;

	std::vector<glm::vec3> parts;

	float m_colors[4] = { 0.33f, 0.33f, 0.33f, 0.f };

	double m_init_mouse_pos_x = 0;
	double m_init_mouse_pos_y = 0;

	double m_world_mouse_pos_x = 0;
	double m_world_mouse_pos_y = 0;
	double m_world_mouse_pos_z = 0;

	float m_cam_velocity = 0.01f;
	float m_cam_sensetivity = 0.5f;

	bool m_isInversiveMouseY = false;
};