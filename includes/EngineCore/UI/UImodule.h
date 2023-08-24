#pragma once

struct GLFWwindow;

class UIlayout;

class UImodule
{
public:
	static void on_window_create(GLFWwindow* pWindow);

	static void on_window_close();

	static void on_window_update_begin();

	static void on_window_update_draw();

	static void on_ui_layout_draw(UIlayout* layout);

	static void show_example_app_dock_space(bool* p_open);
};