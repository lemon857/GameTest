#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec2 position, glm::vec2 size, float zNear, float zFar)
	: m_type(Orthographic)
	, m_eOrentation(Physics::Up)
	, IGameObject(nullptr, "camera", position, size, glm::vec2(0), 0.2)
{
	m_projectionMatrix = glm::ortho(
		m_position.x - (m_size.x / 2),
		m_position.x - (m_size.x / 2) + m_size.x,
		m_position.y - (m_size.y / 2),
		m_position.y - (m_size.x / 2) + m_size.y,
		-100.f, 100.f);
}
Camera::Camera(glm::vec2 position, float fov, float zNear, float zFar) 
	: m_type(Perspective)
	, m_eOrentation(Physics::Up)
	, IGameObject(nullptr, "camera", position, glm::vec2(0), glm::vec2(0), 0.2)
{
	m_projectionMatrix = glm::perspective(90.f, 1.f, zNear, zFar);
}
void Camera::addShaderProgram(std::shared_ptr<RenderEngine::ShaderProgram>& program)
{
	program->use();
	program->setMatrix4("projectionMat", m_projectionMatrix);
	m_shaderPrograms.push_back(program);
}
void Camera::update(const double delta)
{
	updateComponents(delta);
	m_projectionMatrix = glm::ortho(
		m_position.x - (m_size.x / 2),
		m_position.x - (m_size.x / 2) + m_size.x,
		m_position.y - (m_size.y / 2),
		m_position.y - (m_size.x / 2) + m_size.y, 
		-100.f, 100.f);
	for (auto curProgram : m_shaderPrograms)
	{
		curProgram->use();
		curProgram->setMatrix4("projectionMat", m_projectionMatrix);
	}
}