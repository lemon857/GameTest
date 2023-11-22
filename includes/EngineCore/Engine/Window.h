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
	Window(std::string title, glm::ivec2& window_position, glm::ivec2& window_size, bool maximized);
	~Window();

	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	void on_update();
	void maximize();
	void set_pos(glm::ivec2& pos);

	glm::ivec2& get_size() { return m_data.window_size; };
	glm::vec2 get_current_cursor_position() const;

	void set_event_callback(const EventCallback& callback);

private:
	struct WindowData
	{
		std::string title;
		glm::ivec2 window_size;
		glm::ivec2 window_position;
		bool maximized;
		EventCallback event_callback;
	};

	GLFWwindow* m_pWindow;
	WindowData m_data;

	int init();

	void shuitdown();
};