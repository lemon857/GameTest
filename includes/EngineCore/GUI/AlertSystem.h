#pragma once

#include <string>

#include <EngineCore/System/List.h>

#include <glm/vec3.hpp>

namespace RenderEngine
{
	class Material;
	class ShaderProgram;
}

namespace GUI
{
	class Font;
	class Sprite;
	class Button;
	class TextRenderer;
	class GUI_element;

	struct Alert
	{
		Button* button;
		TextRenderer* message;
		Sprite* sprite;
		GUI_element* button_text;
		GUI_element* button_bg;
	};

	class AlertSystem
	{
	public:
		~AlertSystem() = delete;
		AlertSystem() = delete;

		AlertSystem(const AlertSystem&) = delete;
		AlertSystem(const AlertSystem&&) = delete;
		AlertSystem& operator=(const AlertSystem&) = delete;
		AlertSystem& operator=(const AlertSystem&&) = delete;

		static void setResources(std::string font_name, std::string font_name_button, std::string text_shader, std::string sprite_material,
			std::string button_material, glm::vec3 text_color, glm::vec3 text_color_button, bool* pPausegame = nullptr);

		static void addAlert(std::wstring message, std::string soundName = "");
		static void unloadAllalerts();

		static void render();

		static void on_resize();
		static void on_mouse_press(int x, int y);
		static void on_mouse_release(int x, int y);
	private:
		static bool m_close_last;
		static bool* m_pause_game;
		static linked_list<Alert*> m_alerts;
		static std::wstring m_last;
		static std::string m_font_name;
		static std::string m_font_name_button;
		static std::string m_text_shader;
		static std::string m_sprite_material;
		static std::string m_button_material;
		static glm::vec3 m_text_color;
		static glm::vec3 m_text_color_button;
	};
}