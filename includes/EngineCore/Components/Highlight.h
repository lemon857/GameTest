#pragma once

#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include "EngineCore/IComponent.h"

namespace RenderEngine
{
	class Line;
	class ShaderProgram;
}

class Highlight : public IComponent
{
public:
	Highlight(std::shared_ptr<RenderEngine::ShaderProgram>& program, glm::vec3& color);

	void update(const double delta) override;

	void set_active(bool isActive);

private:
	bool m_isActive;
	std::unique_ptr<RenderEngine::Line> m_line;
	glm::vec3 m_color;
};