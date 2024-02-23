#pragma once

#include "EngineCore/System/Event.h"

#include <glm/vec2.hpp>
#include <string>
#include <functional>

struct GLFWwindow;

struct ResizeWindowEvent
{
	unsigned int width;
	unsigned int height;
};

using EventCallback = std::function<void(BaseEvent&)>;

class Window
{
public:
	Window(std::string title, glm::ivec2& window_position, glm::ivec2& window_size, bool maximized, bool fullscreen, std::string path_icon_png = "");
	~Window();

	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	void on_update();
	void maximize();
	void set_pos(glm::ivec2& pos);

	void set_cursor_visible(bool isVisible);
	void set_fullscreen(bool isFullscreen);
	bool is_fullscreen();

	void set_vsync(bool enable);

	glm::ivec2& get_size() { return m_data.window_size; }
	glm::ivec2& get_pos() { return m_data.window_position; }
	bool is_maximized() { return m_data.maximized; }
	glm::vec2 get_current_cursor_position() const;

	void set_event_callback(const EventCallback& callback);

private:
	struct WindowData
	{
		std::string title;
		glm::ivec2 window_size;
		glm::ivec2 window_position;
		bool maximized;
		bool fullscreen;
		EventCallback event_callback;
	};
	std::string m_path_icon_png;
	GLFWwindow* m_pWindow;
	WindowData m_data;

	int init();

	void shuitdown();
};