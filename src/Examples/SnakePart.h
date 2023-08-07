#pragma once

#include <glm/vec2.hpp>
#include <memory>

class SnakePart
{
public:
	SnakePart(glm::ivec2 position);

	void setPostion(glm::ivec2 position);
	glm::ivec2 getPosition();
	void addNext(std::shared_ptr<SnakePart> next);
	std::shared_ptr<SnakePart> getNext();
private:
	glm::ivec2 m_position;
	std::shared_ptr<SnakePart> m_pNext;
};