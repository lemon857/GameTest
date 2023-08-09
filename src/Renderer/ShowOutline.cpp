#include "EngineCore/Renderer/ShowOutline.h"

#include "EngineCore/IGameObject.h"

#include <iostream>

ShowOutline::ShowOutline(IGameObject& targetObj, std::shared_ptr<RenderEngine::ShaderProgram>& program, glm::vec4& color)
	: IComponent(targetObj)
	, m_color(color)
	, m_line(std::make_unique<RenderEngine::Line>(program))
{
}
void ShowOutline::render()
{
	glm::vec2 pos = m_targetObj.getPosition();
	glm::vec2 size = m_targetObj.getSize();
	m_line->render(pos, glm::vec2(0.f, size.y), 1, m_color);
	m_line->render(pos, glm::vec2(size.x, 0.f), 1, m_color);
	m_line->render(glm::vec2(pos.x + size.x, pos.y), glm::vec2(0.f, size.y), 1, m_color);
	m_line->render(glm::vec2(pos.x, pos.y + size.y), glm::vec2(size.x, 0.f), 1, m_color);
}