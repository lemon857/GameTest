#pragma once

#include <glm/vec2.hpp>
#include <string>

struct GLFWwindow;

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

	glm::ivec2& get_size() { return m_window_size; };

	int init();

	void shuitdown();

private:
	GLFWwindow* m_pWindow;
	std::string m_title;
	glm::ivec2 m_window_size;
};