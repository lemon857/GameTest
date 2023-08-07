#include "SnakePart.h"

SnakePart::SnakePart(glm::ivec2 position)
	: m_position(position)
{
}

void SnakePart::setPostion(glm::ivec2 position)
{
	if (m_pNext)
	{
		m_pNext->setPostion(m_position);
	}
	m_position = position;
}

glm::ivec2 SnakePart::getPosition()
{
	return m_position;
}

void SnakePart::addNext(std::shared_ptr<SnakePart> next)
{
	m_pNext = std::move(next);
}

std::shared_ptr<SnakePart> SnakePart::getNext()
{
	return std::move(m_pNext);
}
