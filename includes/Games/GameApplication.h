#pragma once 

#include "EngineCore/Application.h"

#include "EngineCore/Renderer/Camera.h"
#include "EngineCore/Renderer/Line.h"
#include "EngineCore/Resources/Scene.h"

#include <functional>
#include <vector>
#include <glm/vec3.hpp>
	
namespace GUI
{
	class GUI_place;
}

class GameApp : public Application
{
public:
	GameApp();
	~GameApp();

	bool init() override;
	void on_key_update(const double delta) override;
	void on_update(const double delta) override;
	void on_ui_render() override;
	bool init_events() override;

	void set_init(std::function<void()> a) { m_func = a; };
	void set_update(std::function<void()> a) { m_funcUpdate = a; };
private:
	Scene m_scene;

	GUI::GUI_place* m_gui_place;
	Camera* m_cam;
	RenderEngine::Line* m_line;

	std::vector<glm::vec3> parts;

	std::function<void()> m_func;
	std::function<void()> m_funcUpdate;

	float m_colors[4] = { 0.33f, 0.33f, 0.33f, 0.f };

	double m_init_mouse_pos_x = 0;
	double m_init_mouse_pos_y = 0;

	double m_mouse_pos_x = 0;
	double m_mouse_pos_y = 0;

	double m_world_mouse_pos_x = 0;
	double m_world_mouse_pos_y = 0;
	double m_world_mouse_pos_z = 0;

	float m_cam_velocity = 0.01f;
	float m_cam_sensetivity = 0.5f;

	bool m_isInversiveMouseY = false;
};