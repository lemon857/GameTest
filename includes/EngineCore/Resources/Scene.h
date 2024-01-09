#pragma once

#include <vector>
#include <string>

#include "EngineCore/IGameObject.h"
#include "EngineCore/System/List.h"

struct ObjectsPart
{
	unsigned int m_ID;
	IGameObject* object;
};

class Scene
{
public:
	Scene() {};
	~Scene() {};
	
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) = delete;

	linked_list<ObjectsPart*> get_readonly_list() const { return m_objs; };

	linked_list<ObjectsPart*>& get_list() { return m_objs; };

	IGameObject* at(const size_t index) { return m_objs[index]->object; }

	std::vector<std::string> get_items_str() { return m_items_str; };

	void clear();

	template <class _Ty, class... _Types>
	void add_object(_Types&&... _Args)
	{
		IGameObject* obj = (IGameObject*)(new _Ty(std::forward<_Types>(_Args)...));
		m_items_str.push_back(obj->get_name());
		ObjectsPart* part = new ObjectsPart({ 1, std::move(obj) });
		m_objs.push_back(part);
	}
private:

	linked_list<ObjectsPart*> m_objs;

	std::vector<std::string> m_items_str;
};