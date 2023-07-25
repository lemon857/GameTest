#pragma once

#include <unordered_set>
#include <memory>
#include <map>
#include<glm/vec2.hpp>

class IGameObject;

namespace Physics
{
	class Collider;	

	class PhysicsEngine
	{
	public:
		~PhysicsEngine() = delete;
		PhysicsEngine() = delete;

		PhysicsEngine(const PhysicsEngine&) = delete;
		PhysicsEngine(const PhysicsEngine&&) = delete;
		PhysicsEngine& operator=(const PhysicsEngine&) = delete;
		PhysicsEngine& operator=(const PhysicsEngine&&) = delete;

		// Инициализация физического движка
		static void init(bool useGravity = false);
		static void addDynamicObj(std::shared_ptr<IGameObject> obj, int id);
		static void addCollider(std::shared_ptr<Collider> collider, int id);
		static void terminate();
		static void update(const double delta);

	private:
		static bool hasCollidersIntersection(const std::shared_ptr<Collider> collider1, const std::shared_ptr<Collider> collider2);

		typedef std::map<const int, std::shared_ptr<IGameObject>> objMap;
		static objMap m_dynamicObj;

		typedef std::map<const int, std::shared_ptr<Collider>> ColliderMap;
		static ColliderMap m_colliders;

		static double m_g;
		static int m_currentID;

		static bool m_useGravity;
	};
}