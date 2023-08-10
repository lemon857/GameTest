#pragma once

#include "EngineCore/Event.h"
#include "EngineCore/Renderer/Camera.h"

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
	Window(std::string title, glm::ivec2& window_size);
	~Window();

	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	void on_update();

	glm::ivec2& get_size() { return m_data.window_size; };

	void set_event_callback(const EventCallback& callback);

private:
	struct WindowData
	{
		std::string title;
		glm::ivec2 window_size;
		EventCallback event_callback;
	};

	GLFWwindow* m_pWindow;
	WindowData m_data;

	Camera* m_cam;

	int init();

	void shuitdown();
};