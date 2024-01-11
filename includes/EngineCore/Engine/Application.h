#pragma	once
#include "EngineCore/System/Event.h"
#include "EngineCore/System/INI_loader.h" 

#include <memory>
#include <glm/vec2.hpp>

struct INIregionSTARTUP : BaseINIregion
{
	INIregionSTARTUP(glm::ivec2* size, glm::ivec2* pos, bool* max, bool* fs)
		: window_position(std::move(pos))
		, window_size(std::move(size))
		, maximized_window(std::move(max))
		, fullscreen(std::move(fs))
	{

	}
	glm::ivec2* window_size;
	glm::ivec2* window_position;
	bool* maximized_window;
	bool* fullscreen;

	void parse(std::string name, std::string value) override;
	std::string get_str_data() override;
};

class Stopwatch;

class Application
{
public:
	Application();
	~Application();

	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	int start(glm::ivec2& window_size, const char* title, const char* json_rel_path, const char* ini_rel_path, const char* icon_rel_path, double tps_max = 20);

	void stop();
	
protected:
	virtual bool init() { return true; };

	virtual void terminate() {};

	virtual bool init_events() { return true; };

	virtual void on_render() {};

	virtual void on_update(const double delta) {};

	virtual void on_key_update(const double delta) {};

	virtual void on_ui_render() {};

	EventDispatcher m_event_dispather;
	std::unique_ptr<class Window> m_pWindow;
	bool m_pCloseWindow = true;
	bool m_maximized_window = false;
	bool m_fullscreen_window = false;
	glm::ivec2& m_window_position = glm::ivec2(100);
	glm::ivec2& m_window_size = glm::ivec2(800, 600);
	INIdata m_ini_data;

	int m_current_fps;
	int m_current_tps;

	void set_max_tps(double max);
private:
	int m_frames;
	double m_time_frame;

	int m_ticks;
	double m_time_tick;

	double m_max_time_tps;
	Stopwatch* m_watch;
};