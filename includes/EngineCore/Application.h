#pragma	once

#include "EngineCore/Event.h"

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

	virtual bool init() { return false; };

	virtual void on_update(const double delta) {};
protected:
	EventDispatcher m_event_dispather;
	std::unique_ptr<class Window> m_pWindow;
	bool m_pCloseWindow = true;
};