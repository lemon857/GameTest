#pragma once

#include <vector>
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "ShaderProgram.h"
#include "../Engine/IComponent.h"
#include "Line.h"

namespace RenderEngine
{
	class Line;
}

class ShowOutline : public IComponent
{
public:
	ShowOutline(IGameObject& targetObj, std::shared_ptr<RenderEngine::ShaderProgram>& program, glm::vec4& color);

	void render();

private:
	std::unique_ptr<RenderEngine::Line> m_line;
	glm::vec4 m_color;
};