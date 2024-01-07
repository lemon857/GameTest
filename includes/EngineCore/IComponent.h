#pragma once
// Интерфейс для создания компонентов для игровых объектов

class IGameObject;

class IComponent
{
public:
	IComponent(const IComponent&) = delete;
	IComponent& operator=(const IComponent&) = delete;

	virtual void render() {};

	virtual void update(const double delta) {};

	void set_target_object(IGameObject* targetObj) { m_targetObj = targetObj; };

protected:
	IComponent()
		: m_targetObj(nullptr) 
	{};
	IGameObject* m_targetObj;
};