#pragma	once

#include "EngineCore/Event.h"
#include "EngineCore/Renderer/Camera.h"

#include <memory>
#include <glm/vec2.hpp>

class Application
{
public:
	Application();
	virtual  ~Application();

	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	int start(glm::ivec2& window_size, const char* title);

	virtual bool init() { return true; };

	virtual void on_update(const double delta);
protected:
	EventDispatcher m_event_dispather;
	std::unique_ptr<class Window> m_pWindow;
	bool m_pCloseWindow = true;

	Camera* m_cam;

	float m_colors[4] = { 0.f, 0.f, 0.f, 0.f };
	float m_cam_pos[3] = { -5.f, 0.f, 0.f };
	float m_cam_rot[3] = { 0.f, 0.f, 0.f };

	float m_sprite_pos[3] = { 0.f, 0.f, 0.f };

	float m_cam_velocity = 0.01f;
	float m_cam_rotate_velocity = 0.1f;

	bool m_isPerspectiveCam = true;
};