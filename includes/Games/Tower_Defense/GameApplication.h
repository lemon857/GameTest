#pragma once 

#define BUILD_NAME "Tower_defense_0.0.3_alfa"

#include "EngineCore/Engine/Application.h"

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

enum GameMode
{
	Single,
	Multi
};

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

	void start_game_single();
	void start_game_multi();

	void init_winSock();

	std::string m_nickname;
	std::string m_nickname_connect;

	Scene m_scene;

	Castle* m_main_castle;
	Castle* m_adv_castle;

	linked_list<BaseEnemy*> m_enemies;
	linked_list<BaseEnemy*> m_enemies_self;

	std::vector<BaseTower*> m_towers;
	std::vector<BaseTower*> m_towers_self;

	std::queue<std::string> m_chat_mes;

	std::queue<unsigned int> m_spawn_towers;
	std::queue<unsigned int> m_spawn_towers_self;

	std::queue<unsigned int> m_spawn_enemies;
	std::queue<unsigned int> m_spawn_enemies_self;

	Camera* m_cam;

	RenderEngine::Line* m_grid_line;

	std::vector<glm::vec3> parts;
	
	GameMode m_mode = GameMode::Single;

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

	unsigned int cur = 0;
	unsigned int cur_player = 0;
	int curObj = 3;

	int countEnemies = 0;
	int countEnemiesPerm = 0;

	bool is_event_logging_active = false;
	bool is_grid_active = false;
	bool is_gui_active = false;
	bool is_chat_active = false;
	bool is_chat_full_hide = false;
	bool is_debug_active = false;
	bool is_spawn_enemy = false;
	bool is_spawn_mode = false;
	bool is_lock_move = true;
	bool is_mute = false;

	bool isKeyPressed = false;
	bool isKeyPressedmouse = false;

	bool isServer = false;
	bool restart_querry = false;
	
	bool* lock_key_update;

	float volume = 70;

	unsigned int countKills = 0;
	unsigned int fps = 0;
	unsigned int frames = 0;
	double times = 0;

	double angle = 0;

	std::vector<std::string> chat_last; // need optimization

	size_t cur_chat_index = 0;

	const unsigned int max_buffer_chat_last = 32;

	// settings
	double _set_min_distance = 7;
	double _set_velosity = 7;
	double _set_max_hp_castle = 100;
	double _set_max_hp_enemy = 50;
	double _set_cooldown_tower = 3;
	unsigned int _set_damage_tower = 10;

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