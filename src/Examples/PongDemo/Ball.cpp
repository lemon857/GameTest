#include "Ball.h"

Ball::Ball(std::shared_ptr<RenderEngine::ShaderProgram>& program,
	const float sizePoint,
	const glm::vec2& position,
	const double velocity)
	: PongObject(nullptr, "ball", position, glm::vec2(1), velocity)
	, m_point(std::move(std::make_shared<RenderEngine::Point>(program, sizePoint)))
{
	//m_move = true;
}

void Ball::render()
{
	//m_point->render(m_position, 2, glm::vec4(1));
}
