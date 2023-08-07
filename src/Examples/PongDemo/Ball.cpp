#include "Ball.h"

Ball::Ball(std::shared_ptr<RenderEngine::ShaderProgram>& program,
	const float sizePoint,
	const glm::vec2& position)
	: PongObject(nullptr, "ball", position, glm::vec2(0))
	, m_point(std::move(std::make_shared<RenderEngine::Point>(program, sizePoint)))
{

}
