#pragma once

#include <glm/vec2.hpp>

class IGameObject 
{
public:
	IGameObject(const glm::vec2& position, const glm::vec2& size, const glm::vec2& moveOffset, const double velocity, const double weight)
		: m_velocity(velocity),
		m_weight(weight),
		m_position(position),
		m_size(size),
		m_moveOffset(moveOffset)
	{};
	virtual void update(double delta) {};
	bool isMove() const { return m_move; };
	double getVelocity() { return m_velocity; };
	double getWeight() { return m_weight; };
	glm::vec2& getSize() { return m_size; };
	glm::vec2& getPosition() { return m_position; };
	glm::vec2& getOffset() { return m_moveOffset; };
protected:
	glm::vec2 m_position;
	glm::vec2 m_size;
	glm::vec2 m_moveOffset;

	double m_weight;
	double m_velocity;
	bool m_move = false;
};