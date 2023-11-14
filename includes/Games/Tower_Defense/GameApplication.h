#pragma once 

#include "EngineCore/Application.h"

#include "EngineCore/Renderer/Camera.h"
#include "EngineCore/Renderer/Line.h"
#include "EngineCore/Resources/Scene.h"

#include <vector>
#include <glm/vec3.hpp>	
#include <queue>

#include "EngineCore/System/List.h"

#include "Games/Tower_Defense/Castle.h"
#include "Games/Tower_Defense/BaseEnemy.h"
#include "Games/Tower_Defense/BaseTower.h"
#include "Games/Tower_Defense/BaseEffect.h"

const int size_x = 30, size_y = 30;

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
private:
	void init_gui();

	void start_game();

	Scene m_scene;

	Castle* m_main_castle;

	linked_list<BaseEnemy*> m_enemies;

	std::vector<BaseTower*> m_towers;

	std::queue<std::string> m_chat_mes;

	Camera* m_cam;

	RenderEngine::Line* m_grid_line;

	std::vector<glm::vec3> parts;

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
	bool m_isLose = false;

	std::array <bool, size_x * size_y> map;

	int cur = 0;
	int curObj = 3;

	int countEnemies = 0;
	int countEnemiesPerm = 0;

	bool is_event_logging_active = false;

	bool is_grid_active = false;

	bool is_gui_active = false;

	bool is_chat_active = true;

	bool is_debug_active = false;

	bool isKeyPressed = false;
	bool isKeyPressedmouse = false;

	unsigned int countKills = 0;
	unsigned int fps = 0;
	unsigned int frames = 0;
	double times = 0;

	double angle = 0;

	GUI::GUI_place* m_gui;
	GUI::GUI_place* m_gui_chat;
	GUI::GUI_place* m_gui_debug;
	GUI::GUI_place* m_gui_place_menu;
	GUI::GUI_place* m_gui_place_settings;

	enum GUI_Active
	{
		null,
		main,
		settings
	};

	GUI_Active gui_window;
};