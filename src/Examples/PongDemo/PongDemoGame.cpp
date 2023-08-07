#include "PongDemoGame.h"

#include "../../Renderer/ShaderProgram.h"
#include "../../Renderer/Texture2D.h"
#include "../../Renderer/Sprite.h"
#include "../../Renderer/ShowOutline.h"
#include "../../Resources/ResourceManager.h"
#include "../../Physics/PhysicsEngine.h"
#include "../../Physics/Collider.h"
#include "../../Physics/CharacterController.h"

#define WALL_OFFSET 20


void onCollisionPlate(IGameObject& targetObj, std::string name, Physics::EDirection dir)
{
	if (name == "wall")
	{
		switch (dir)
		{
		case Physics::Up:
			if (targetObj.getMoveOffset().y > 0) targetObj.setMoveOffset(glm::vec2(targetObj.getMoveOffset().x, 0.f));
			break;
		case Physics::Down:
			if (targetObj.getMoveOffset().y < 0) targetObj.setMoveOffset(glm::vec2(targetObj.getMoveOffset().x, 0.f));
			break;
		case Physics::Left:
			if (targetObj.getMoveOffset().x < 0) targetObj.setMoveOffset(glm::vec2(0.f, targetObj.getMoveOffset().y));
			break;
		case Physics::Right:
			if (targetObj.getMoveOffset().x > 0) targetObj.setMoveOffset(glm::vec2(0.f, targetObj.getMoveOffset().y));
			break;
		}
	}
}

PongDemoGame::PongDemoGame(glm::ivec2 windowSize)
	: IGame(windowSize)
	, m_status(Active)
{
}

void PongDemoGame::render() const
{
	m_downWall->render();
	m_upWall->render();
	m_plate1->render();
	//m_ball->render(glm::vec2(100.f, 100.f), 2, glm::vec4(1));
}

void PongDemoGame::update(const double delta)
{
	if (m_keys[GLFW_KEY_W])
	{
		m_plate1->setOrentation(Physics::EDirection::Up);
		m_plate1->move(true);
	}
	else if ((m_keys[GLFW_KEY_S]))
	{
		m_plate1->setOrentation(Physics::EDirection::Down);
		m_plate1->move(true);
	}
	else
	{
		m_plate1->move(false);
	}
	m_plate1->update(delta);
}

bool PongDemoGame::init()
{
	ResourceManager::loadJSONresources("res/resources.json");

	auto pShapeShader = ResourceManager::getShaderProgram("shapeShader");

	m_ball = std::make_shared<Ball>(pShapeShader, 15.f, m_WindowSize / 2);

	m_cam = std::make_shared<Camera>(glm::vec2(0) + (glm::vec2)(m_WindowSize / 2), m_WindowSize, -100.f, 100.f);

	m_cam->addShaderProgram(pShapeShader);

	m_downWall = std::make_shared<PongObject>(nullptr, "wall", glm::vec2(-1.f), glm::vec2(m_WindowSize.x + 2.f, WALL_OFFSET));
	m_upWall = std::make_shared<PongObject>(nullptr, "wall", glm::vec2(0.f, m_WindowSize.y - 100.f - WALL_OFFSET), glm::vec2(m_WindowSize.x + 2.f, WALL_OFFSET));

	m_plate1 = std::make_shared<PongObject>(nullptr, "plate", glm::vec2(10.f, 100.f), glm::vec2(20.f, 100.f), 0.6);

	m_downWall->addComponent("showOutline", std::make_shared<ShowOutline>(*m_downWall, pShapeShader, glm::vec4(1)));
	m_upWall->addComponent("showOutline", std::make_shared<ShowOutline>(*m_upWall, pShapeShader, glm::vec4(1)));
	m_plate1->addComponent("showOutline", std::make_shared<ShowOutline>(*m_plate1, pShapeShader, glm::vec4(1)));

	m_plate1->addComponent("characterController", std::make_shared<CharacterController>(*m_plate1));

	auto plate1Col = std::make_shared<Physics::Collider>(*m_plate1);
	auto downWallCol = std::make_shared<Physics::Collider>(*m_downWall);
	auto upWallCol = std::make_shared<Physics::Collider>(*m_upWall);

	m_plate1->addComponent("collider", plate1Col);
	m_downWall->addComponent("collider", downWallCol);
	m_upWall->addComponent("collider", upWallCol);

	Physics::PhysicsEngine::addCollider(plate1Col);
	Physics::PhysicsEngine::addCollider(upWallCol);
	Physics::PhysicsEngine::addCollider(downWallCol);

	plate1Col->setOnCollisionCallback(onCollisionPlate);

	return true;
}
