#pragma once

#include <unordered_set>
#include <memory>

class IGameObject;

class PhysicsEngine 
{
public:
	~PhysicsEngine() = delete;
	PhysicsEngine() = delete;

	PhysicsEngine(const PhysicsEngine&) = delete;
	PhysicsEngine(const PhysicsEngine&&) = delete;
	PhysicsEngine& operator=(const PhysicsEngine&) = delete;
	PhysicsEngine& operator=(const PhysicsEngine&&) = delete;

	static void init();
	static void addDynamicObj(std::shared_ptr<IGameObject> obj);
	static void terminate();
	static void update(const double delta);

private:
	static std::unordered_set<std::shared_ptr<IGameObject>> m_dynamicObj;
	static double m_g;
};