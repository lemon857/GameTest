#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec2 position, glm::vec2 size, float zNear, float zFar)
	: m_type(Orthographic)
	, m_eOrentation(Physics::Up)
	, IGameObject(nullptr, "camera", position, size, glm::vec2(0), 0.2)
{
	m_projectionMatrix = glm::ortho(position.x, size.x, position.y, size.y, zNear, zFar);
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
	m_projectionMatrix = glm::ortho(m_position.x, m_position.x + m_size.x, m_position.y, m_position.y + m_size.y, -100.f, 100.f);
	for (auto curProgram : m_shaderPrograms)
	{
		curProgram->use();
		curProgram->setMatrix4("projectionMat", m_projectionMatrix);
	}
}
void Camera::move(bool isMove)
{
	m_move = isMove;
}
void Camera::setOrentation(const Physics::EDirection orentation)
{
	if (m_eOrentation == orentation) return;
	m_eOrentation = orentation;
	switch (m_eOrentation)
	{
	case Physics::EDirection::Up:
		//m_pSprite->setState("TopState");
		//m_pAnimator->startAnimation("TopMove");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = 1.f;
		break;
	case Physics::EDirection::Down:
		//m_pSprite->setState("BottomState");
		//m_pAnimator->startAnimation("BottomMove");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = -1.f;
		break;
	case Physics::EDirection::Left:
		//m_pSprite->setState("LeftState");
		//m_pAnimator->startAnimation("LeftMove");
		m_moveOffset.x = -1.f;
		m_moveOffset.y = 0.f;
		break;
	case Physics::EDirection::Right:
		//m_pSprite->setState("RightState");
		//m_pAnimator->startAnimation("RightMove");
		m_moveOffset.x = 1.f;
		m_moveOffset.y = 0.f;
		break;
	default:
		//m_pSprite->setState("TopState");
		//m_pAnimator->startAnimation("TopMove");
		m_moveOffset.x = 0.f;
		m_moveOffset.y = 1.f;
		break;
	}
}