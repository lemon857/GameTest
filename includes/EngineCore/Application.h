#pragma	once
#include "EngineCore/Event.h"
#include "EngineCore/IGameObject.h"
#include "EngineCore/System/List.h"

#include <memory>
#include <glm/vec2.hpp>
#include <list>

class Application
{
public:
	Application();
	virtual  ~Application();

	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	int start(glm::ivec2& window_size, const char* title, const char* json_rel_path, const char* ini_rel_path);

	virtual bool init() { return true; };

	virtual bool init_events() { return true; };

	virtual void on_update(const double delta) {};

	virtual void on_key_update(const double delta) {};

	virtual void on_ui_render() {};
	// TEMP
	template <class _Ty, class... _Types>
	void add_object(_Types&&... _Args)
	{
		IGameObject* obj = (IGameObject*)(new _Ty(std::forward<_Types>(_Args)...));
		m_items_str.push_back(obj->get_name());
		m_objs.push_back(std::move(obj));
	}
protected:
	EventDispatcher m_event_dispather;
	std::unique_ptr<class Window> m_pWindow;
	bool m_pCloseWindow = true;
	bool m_maximized_window = false;
	glm::ivec2& m_window_position = glm::ivec2(100);

	std::vector<std::string> m_items_str;

	linked_list<IGameObject*> m_objs;
};