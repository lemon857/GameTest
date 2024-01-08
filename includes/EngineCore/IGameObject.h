#pragma once
// Интерфейс для создания игровых объектов

#include <string>
#include <map>

#include "EngineCore/IComponent.h"
#include "EngineCore/System/List.h"

class IGameObject
{
public:
	virtual void update(const double delta) { updateComponents(delta); }

	virtual void render() { renderComponents(); }

	std::string get_name() { return m_name; }

	void set_name(const std::string name) { m_name = name; }

	// Возврат nullptr означает уже наличие добавляемого компонента, либо неудачное создание компонента
	template <class _Ty, class... _Types>
	_Ty* addComponent(_Types&&... _Args)
	{
		if (getComponent<_Ty>() != nullptr) return nullptr;
		IComponent* component = (IComponent*)(new _Ty(std::forward<_Types>(_Args)...));
		component->set_target_object(this);
		m_components.emplace(typeid(_Ty).name(), component);
		m_com_render.push_back(component);
		return (_Ty*)component;
	}

	template <class _Ty>
	_Ty* getComponent()
	{
		componentsMap::const_iterator it = m_components.find(typeid(_Ty).name());
		if (it != m_components.end())
		{
			return (_Ty*)it->second;
		}
		return nullptr;
	}

	template <class _Ty>
	void deleteComponent()
	{
		for (auto pos = m_components.begin(); pos != m_components.end();)
		{
			if (pos->first == typeid(_Ty).name())
			{
				m_com_render.remove(pos->second);
				delete pos->second;
				pos = m_components.erase(pos);
			}
			else
			{
				++pos;
			}
		}
	}
protected:
	IGameObject(const std::string name = "default")
		: m_name(name)
	{

	};
	~IGameObject()
	{
		m_components.clear();
	};

	void updateComponents(double delta)
	{
		for (auto curCom : m_components)
		{
			curCom.second->render();
		}
	};
	void renderComponents()
	{
		for (int i = 0; i < m_com_render.size(); i++)
		{
			m_com_render.at(i)->render();
		}
	};

private:
	typedef std::map<std::string, IComponent*> componentsMap;
	componentsMap m_components;
	linked_list<IComponent*> m_com_render;
	std::string m_name;
};