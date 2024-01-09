#pragma once 

#include "EngineCore/Engine/Application.h"

#include "EngineCore/Renderer/Camera.h"
#include "EngineCore/Renderer/Line.h"
#include "EngineCore/Renderer/Circle.h"
#include "EngineCore/Resources/Scene.h"

#include <vector>
#include <glm/vec3.hpp>	
#include <queue>

#include "EngineCore/System/List.h"

#include "Games/Tower_Defense/Castle.h"
#include "Games/Tower_Defense/BaseEnemy.h"
#include "Games/Tower_Defense/BaseTower.h"
#include "Games/Tower_Defense/BaseEffect.h"
#include "Games/Tower_Defense/Target.h"

#include "Games/Tower_Defense/TowerArcher.h"
#include "Games/Tower_Defense/TowerIce.h"
#include "Games/Tower_Defense/TowerMortar.h"
#include "Games/Tower_Defense/TowerExecutioner.h"
#include "Games/Tower_Defense/TowerInferno.h"

const int size_x = 30, size_y = 30;

const int count_start_coins = 1000000;

const int time_before_start = 5000;

struct INIregionUSER : BaseINIregion
{
	std::string* nickname;
	int* view_pos;

	void parse(std::string name, std::string value) override;
	std::string get_str_data() override;
};

class ParticleSystem;
class LanguagePack;

enum class KeyCode;

namespace GUI
{
	class GUI_place;
}

enum class TypeTower
{
	null = 0,
	Ice = IceTower::p_coast,
	Archer = ArcherTower::p_coast,
	Mortar = MortarTower::p_coast,
	Executioner = ExecutionerTower::p_coast,
	Inferno = InfernoTower::p_coast
};
enum class TypeEnemy
{
	null = 0,
	Monkey,
	Magician,
	Robot,
	Bug,
	Professor
};

struct WaveInfo
{
	double time_before;
	int count_monkeys;
	int count_magicans;
};

struct SpawnTower
{
	unsigned int num;
	TypeTower type;
};

struct SpawnEnemy
{
	unsigned int num;
	TypeEnemy type;
};


struct Tower
{
	BaseTower* tow;
	unsigned int num;
};

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

private:
	bool init() override;
	void terminate() override;
	void on_key_update(const double delta) override;
	void on_update(const double delta) override;
	void on_render(const double delta) override;
	void on_ui_render() override;
	bool init_events() override;

	void press_button(KeyCode key);

	void init_gui();

	void start_game_single();
	void start_game_multi();

	void init_winSock();

	void add_green_place(glm::vec3 startPos, glm::vec3 endPos);

	std::string m_nickname;
	//std::string m_nickname_connect;

	Scene m_scene;

	Castle* m_main_castle;
	//Castle* m_adv_castle;

	linked_list<BaseEnemy*> m_enemies;
	linked_list<BaseEnemy*> m_enemies_broken;
	//linked_list<BaseEnemy*> m_enemies_self;

	std::vector<Tower> m_towers;
	BaseTower* m_select_tower = nullptr;
	//std::vector<BaseTower*> m_towers_self;

	std::queue<std::wstring> m_chat_mes;

	std::queue<SpawnTower> m_spawn_towers;
	//std::queue<unsigned int> m_spawn_towers_self;

	std::queue<SpawnEnemy> m_spawn_enemies;
	//std::queue<unsigned int> m_spawn_enemies_self;

	linked_list<ParticleSystem*> m_psystems;

	Camera* m_cam;

	RenderEngine::Circle* m_circle;

	RenderEngine::Line* m_line;

	std::vector<glm::vec3> parts;

	std::vector<Target> targets;

	std::vector<WaveInfo> waves;
	int cur_wave = -1;

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

	std::array <bool, size_x* size_y> map;
	glm::vec3 size_place;
	unsigned int cur = 0;
	unsigned int cur_player = 0;
	int curObj = 3;

	int countEnemies = 0;
	int countEnemiesPerm = 0;

	bool is_event_logging_active = false;
	bool is_green_place_active = true;
	bool is_grid_active = false;
	bool is_line_active = false;
	bool is_gui_active = false;
	bool is_chat_active = false;
	bool is_chat_full_hide = false;
	bool is_spawn_enemy = false;
	bool is_spawn_mode = false;
	bool is_lock_move = true;
	bool is_special_move = false;
	bool is_mute = false;
	bool is_game_paused = false;
	bool is_active_cursor = false;
	bool is_active_cursor_tmp = false;

	bool moveCamMoue = false;
	bool isKeyPressed = false;
	bool isKeyPressedmouse = false;

	bool isServer = false;
	bool restart_querry = false;

	TypeTower place_querry = TypeTower::null;

	bool* lock_key_update;

	float volume;

	unsigned int countSpawnEnemies = 10;
	unsigned int countKills = 0;
	unsigned int fps = 0;
	unsigned int tps = 0;
	unsigned int frames = 0;
	unsigned int ticks = 0;
	unsigned int g_coins = count_start_coins;
	double times = 0;
	double timesTicks = 0;

	double angle = 0;

	std::vector<std::wstring> chat_last; // need optimization

	size_t cur_chat_index = 0;

	int g_cam_view = 0;
	size_t selected_enemy = -1;

	const unsigned int max_buffer_chat_last = 32;

	// settings
	double _set_max_hp_castle = 100;
	// binds

	KeyCode k_debug = KeyCode::KEY_F3;
	KeyCode k_spawn_en_one = KeyCode::KEY_J;
	KeyCode k_spawn_en_pack = KeyCode::KEY_K;
	KeyCode k_chat = KeyCode::KEY_T;
	KeyCode k_table = KeyCode::KEY_H;
	KeyCode k_view = KeyCode::KEY_Q;

	GUI::GUI_place* m_gui;

	INIregionUSER m_ini_region_user;

	glm::vec3 movDel = glm::vec3(0);
	glm::vec3 rotDel = glm::vec3(0);
	glm::vec3 uPos = glm::vec3(25.f, -10.f, 25.f);

	std::shared_ptr<LanguagePack> m_lang_pack;
};