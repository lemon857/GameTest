#pragma once

#include <memory>

#include "PongObject.h"

#include "../../Renderer/ShaderProgram.h"
#include "../../Renderer/Point.h"

class Ball : public PongObject
{
public:
	Ball(std::shared_ptr<RenderEngine::ShaderProgram>& program,
		const float sizePoint,
		const glm::vec2& position);



private:
	std::shared_ptr<RenderEngine::Point> m_point;
};