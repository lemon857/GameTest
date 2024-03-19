#include "EngineCore/GUI/AlertSystem.h"

#include "EngineCore/GUI/GUI_place.h"
#include "EngineCore/GUI/Button.h"
#include "EngineCore/GUI/Sprite.h"
#include "EngineCore/GUI/TextRenderer.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Resources/LanguagePack.h"
#include "EngineCore/Sound/Sound.h"
#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/System/Log.h"

linked_list<GUI::Alert*> GUI::AlertSystem::m_alerts;
std::wstring GUI::AlertSystem::m_last;
std::string GUI::AlertSystem::m_font_name;
std::string GUI::AlertSystem::m_font_name_button;
std::string GUI::AlertSystem::m_text_shader;
std::string GUI::AlertSystem::m_sprite_material;
std::string GUI::AlertSystem::m_button_material;
glm::vec3 GUI::AlertSystem::m_text_color;
glm::vec3 GUI::AlertSystem::m_text_color_button;
bool GUI::AlertSystem::m_close_first;
bool* GUI::AlertSystem::m_pause_game;

void GUI::AlertSystem::setResources(std::string font_name, std::string font_name_button, std::string text_shader, std::string sprite_material,
	std::string button_material, glm::vec3 text_color_button, glm::vec3 text_color, bool* pPausegame)
{
	m_last = L"";
	m_font_name = font_name;
	m_font_name_button = font_name_button;
	m_text_color = text_color;
	m_text_color_button = text_color_button;
	m_sprite_material = sprite_material;
	m_button_material = button_material;
	m_text_shader = text_shader;
	m_close_first = false;
	m_pause_game = std::move(pPausegame);
}

void GUI::AlertSystem::addAlert(std::wstring message, std::string soundName)
{
	if (m_last == message) return;
	if (m_pause_game != nullptr) *m_pause_game = true;
	LanguagePack* pack = ResourceManager::get_current_lang_pack();
	Alert* a = new Alert;
	a->button = new Button(new Sprite(ResourceManager::getMaterial(m_button_material), "static"),
		glm::vec2(0.f), glm::vec2(0.f), pack->get("submit"),
		ResourceManager::getShaderProgram(m_text_shader), ResourceManager::get_font(m_font_name_button), m_text_color_button);
	a->message = new TextRenderer(ResourceManager::get_font(m_font_name), ResourceManager::getShaderProgram(m_text_shader), 
		message, m_text_color, glm::vec2(0.f), glm::vec2(0.f));
	a->sprite = new Sprite(ResourceManager::getMaterial(m_sprite_material));
	a->button->set_position(GUI_place::get_pix_percent(glm::vec2(50.f, 42.5f)));
	a->button->set_scale(GUI_place::get_pix_percent(glm::vec2(8.f, 3.5f)));
	a->message->set_position(GUI_place::get_pix_percent(glm::vec2(50.f, 59.4f)));
	a->message->set_scale(GUI_place::get_pix_percent(glm::vec2(10.f, 1.f)));
	a->sprite->set_position(GUI_place::get_pix_percent(glm::vec2(50.f)));
	a->sprite->set_scale(GUI_place::get_pix_percent(glm::vec2(10.f, 13.f)));
	
	a->message->set_text(message);

	a->button_bg = a->button->get_elements()[0];
	a->button_text = a->button->get_elements()[1];

	a->button_bg->set_position(GUI_place::get_pix_percent(glm::vec2(50.f, 42.5f)));
	a->button_bg->set_scale(GUI_place::get_pix_percent(glm::vec2(8.f, 3.5f)));
	a->button_text->set_position(GUI_place::get_pix_percent(glm::vec2(50.f, 42.5f)));
	a->button_text->set_scale(GUI_place::get_pix_percent(glm::vec2(8.f, 3.35f)));

	a->button->set_layer(7.f);
	a->button_bg->set_layer(7.f);
	a->button_text->set_layer(7.f);
	a->message->set_layer(7.f);
	a->sprite->set_layer(5.f);

	a->button->set_mouse_up_callback([&]() {
		m_close_first = true;
		});

	if (soundName != "") ResourceManager::get_sound(soundName)->play();

	a->callback = nullptr;

	m_last = message;
	m_alerts.push_back(a);
	LOG_INFO("[AS] Add alert: {0}", message);
}

void GUI::AlertSystem::addAlert(std::wstring message, std::function<void(void)> callback, std::string soundName)
{
	if (m_last == message) return;
	if (m_pause_game != nullptr) *m_pause_game = true;
	LanguagePack* pack = ResourceManager::get_current_lang_pack();
	Alert* a = new Alert;
	a->button = new Button(new Sprite(ResourceManager::getMaterial(m_button_material), "static"),
		glm::vec2(0.f), glm::vec2(0.f), pack->get("submit"),
		ResourceManager::getShaderProgram(m_text_shader), ResourceManager::get_font(m_font_name_button), m_text_color_button);
	a->message = new TextRenderer(ResourceManager::get_font(m_font_name), ResourceManager::getShaderProgram(m_text_shader),
		message, m_text_color, glm::vec2(0.f), glm::vec2(0.f));
	a->sprite = new Sprite(ResourceManager::getMaterial(m_sprite_material));
	a->button->set_position(GUI_place::get_pix_percent(glm::vec2(50.f, 44.f)));
	a->button->set_scale(GUI_place::get_pix_percent(glm::vec2(8.f, 5.f)));
	a->message->set_position(GUI_place::get_pix_percent(glm::vec2(50.f, 55.5f)));
	a->message->set_scale(GUI_place::get_pix_percent(glm::vec2(8.f, 5.f)));
	a->sprite->set_position(GUI_place::get_pix_percent(glm::vec2(50.f)));
	a->sprite->set_scale(GUI_place::get_pix_percent(glm::vec2(10.f, 13.f)));

	a->message->set_text(message);

	a->button_bg = a->button->get_elements()[0];
	a->button_text = a->button->get_elements()[1];

	a->button_bg->set_position(GUI_place::get_pix_percent(glm::vec2(50.f, 44.f)));
	a->button_bg->set_scale(GUI_place::get_pix_percent(glm::vec2(8.f, 5.f)));
	a->button_text->set_position(GUI_place::get_pix_percent(glm::vec2(50.f, 44.f)));
	a->button_text->set_scale(GUI_place::get_pix_percent(glm::vec2(8.f, 5.f)));

	a->button->set_layer(7.f);
	a->button_bg->set_layer(7.f);
	a->button_text->set_layer(7.f);
	a->message->set_layer(7.f);
	a->sprite->set_layer(5.f);

	a->button->set_mouse_up_callback([&]() {
		m_close_first = true;
		});

	if (soundName != "") ResourceManager::get_sound(soundName)->play();

	a->callback = std::move(callback);

	m_last = message;
	m_alerts.push_back(a);
	LOG_INFO("[AS] Add alert: {0}", message);
}

void GUI::AlertSystem::unloadAllalerts()
{
	if (m_alerts.is_empty()) return;
	for (int i = 0; i < m_alerts.size(); i++)
	{
		delete m_alerts[i];
	}
}

void GUI::AlertSystem::render()
{
	if (m_alerts.is_empty()) return;
	if (m_close_first)
	{ 
		delete m_alerts[0]->button;
		delete m_alerts[0]->button_bg;
		delete m_alerts[0]->button_text;
		delete m_alerts[0]->message;
		delete m_alerts[0]->sprite;
		m_alerts.remove_first();
		m_close_first = false;
		m_last = L"";
		if (m_pause_game != nullptr) *m_pause_game = false;
		return;
	}

	m_alerts[0]->button_bg->on_render_prj(GUI_place::get_prj_matrix());
	m_alerts[0]->button_text->on_render_prj(GUI_place::get_prj_matrix());
	m_alerts[0]->message->on_render_prj(GUI_place::get_prj_matrix());
	m_alerts[0]->sprite->on_render_prj(GUI_place::get_prj_matrix());
}

void GUI::AlertSystem::on_resize()
{
	if (m_alerts.is_empty()) return;
	for (int i = 0; i < m_alerts.size(); i++)
	{
		m_alerts[i]->button->set_position(GUI_place::get_pix_percent(glm::vec2(50.f, 44.f)));
		m_alerts[i]->button->set_scale(GUI_place::get_pix_percent(glm::vec2(8.f, 5.f)));
		m_alerts[i]->button_bg->set_position(GUI_place::get_pix_percent(glm::vec2(50.f, 44.f)));
		m_alerts[i]->button_bg->set_scale(GUI_place::get_pix_percent(glm::vec2(8.f, 5.f)));
		m_alerts[i]->button_text->set_position(GUI_place::get_pix_percent(glm::vec2(50.f, 44.f)));
		m_alerts[i]->button_text->set_scale(GUI_place::get_pix_percent(glm::vec2(8.f, 5.f)));
		m_alerts[i]->message->set_position(GUI_place::get_pix_percent(glm::vec2(50.f, 54.f)));
		m_alerts[i]->message->set_scale(GUI_place::get_pix_percent(glm::vec2(8.f, 5.f)));
		m_alerts[i]->sprite->set_position(GUI_place::get_pix_percent(glm::vec2(50.f)));
		m_alerts[i]->sprite->set_scale(GUI_place::get_pix_percent(glm::vec2(10.f, 13.f)));
	}
}

void GUI::AlertSystem::on_mouse_press(int x, int y)
{
	if (m_alerts.is_empty()) return;
	glm::vec2 vp_size = GUI_place::get_vp_size();
	y = vp_size.y - y; // set null pos in left down
	for (int i = 0; i < m_alerts.size(); i++)
	{
		glm::vec2 scale = m_alerts[i]->button->get_scale();
		glm::vec2 pos = m_alerts[i]->button->get_position() - scale;
		scale *= 2;
		if ((x >= pos.x && y >= pos.y && x <= pos.x + scale.x && y <= pos.y + scale.y))
		{
			m_alerts[i]->button->set_pos_mouse_click(glm::vec2(x, y));
			m_alerts[i]->button->on_press();
			if (m_alerts[i]->callback != nullptr) m_alerts[i]->callback();
		}
	}
}

void GUI::AlertSystem::on_mouse_release(int x, int y)
{
	if (m_alerts.is_empty()) return;
	glm::vec2 m_vp_size = GUI_place::get_vp_size();
	y = m_vp_size.y - y; // set null pos in left down
	for (int i = 0; i < m_alerts.size(); i++)
	{		
		m_alerts[i]->button->on_release();
		if (!m_alerts[i]->button->get_active()) continue;
		glm::vec2 scale = m_alerts[i]->button->get_scale();
		glm::vec2 pos = m_alerts[i]->button->get_position() - scale;
		scale *= 2;
		if ((x >= pos.x && y >= pos.y && x <= pos.x + scale.x && y <= pos.y + scale.y))
		{
			m_alerts[i]->button->on_release_hover();
		}
	}
}
