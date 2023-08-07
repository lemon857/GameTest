#include "PongDemoGame.h"

#include "../../Renderer/ShaderProgram.h"
#include "../../Renderer/Texture2D.h"
#include "../../Renderer/Sprite.h"
#include "../../Renderer/ShowOutline.h"
#include "../../Resources/ResourceManager.h"
#include "../../Physics/PhysicsEngine.h"
#include "../../Physics/Collider.h"
#include "../../Physics/CharacterController.h"

#define WALL_OFFSET 10


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

void onCollisionBall(IGameObject& targetObj, std::string name, Physics::EDirection dir)
{
	glm::vec2 offset = targetObj.getMoveOffset();
	switch (dir)
	{
	case Physics::Up:
		targetObj.setMoveOffset(glm::vec2(offset.x, -offset.y));
		break;
	case Physics::Down:
		targetObj.setMoveOffset(glm::vec2(offset.x, -offset.y));
		break;
	case Physics::Left:
		targetObj.setMoveOffset(glm::vec2(-offset.x, offset.y));
		break;
	case Physics::Right:
		targetObj.setMoveOffset(glm::vec2(-offset.x, offset.y));
		break;
	}
	if (name == "plate")
	{
		if (dir == Physics::Left)
		{

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
	m_leftWall->render();
	m_rightWall->render();
	m_plate1->render();
	m_plate2->render();
	m_ball->render();
	//m_text->render(glm::vec2(100.f, 100.f), glm::vec2(100.f, 100.f), 0, 2);
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

	if (m_keys[GLFW_KEY_UP])
	{
		m_plate2->setOrentation(Physics::EDirection::Up);
		m_plate2->move(true);
	}
	else if ((m_keys[GLFW_KEY_DOWN]))
	{
		m_plate2->setOrentation(Physics::EDirection::Down);
		m_plate2->move(true);
	}
	else
	{
		m_plate2->move(false);
	}
	m_plate1->update(delta);
	m_plate2->update(delta);
	m_ball->update(delta);
}

bool PongDemoGame::init()
{
	ResourceManager::loadJSONresources("res/resources.json");

	auto pShapeShader = ResourceManager::getShaderProgram("shapeShader");
	auto pSpriteShader = ResourceManager::getShaderProgram("spriteShader");

	m_ball = std::make_shared<Ball>(pShapeShader, 15.f, m_WindowSize / 2, 0.2);
 
	m_cam = std::make_shared<Camera>(glm::vec2(0) + (glm::vec2)(m_WindowSize / 2), m_WindowSize, -100.f, 100.f);

	m_cam->addShaderProgram(pShapeShader);
	m_cam->addShaderProgram(pSpriteShader);

	m_downWall = std::make_shared<PongObject>(nullptr, "wall", glm::vec2(-1.f), glm::vec2(m_WindowSize.x + 2.f, WALL_OFFSET));
	m_upWall = std::make_shared<PongObject>(nullptr, "wall", glm::vec2(0.f, m_WindowSize.y - 100.f - WALL_OFFSET), glm::vec2(m_WindowSize.x + 2.f, WALL_OFFSET));

	m_leftWall = std::make_shared<PongObject>(nullptr, "deadWall", glm::vec2(-1.f), glm::vec2(WALL_OFFSET, m_WindowSize.y + 1.f));
	m_rightWall = std::make_shared<PongObject>(nullptr, "deadWall", glm::vec2(m_WindowSize.x - WALL_OFFSET, -1.f), glm::vec2(m_WindowSize.x + 1, m_WindowSize.y + 1.f));

	m_plate1 = std::make_shared<PongObject>(nullptr, "plate", glm::vec2(30.f, 100.f), glm::vec2(20.f, 100.f), 0.6);
	m_plate2 = std::make_shared<PongObject>(nullptr, "plate", glm::vec2(750.f, 100.f), glm::vec2(20.f, 100.f), 0.6);

	m_rightWall->addComponent("showOutline", std::make_shared<ShowOutline>(*m_rightWall, pShapeShader, glm::vec4(1)));
	m_leftWall->addComponent("showOutline", std::make_shared<ShowOutline>(*m_leftWall, pShapeShader, glm::vec4(1)));
	m_downWall->addComponent("showOutline", std::make_shared<ShowOutline>(*m_downWall, pShapeShader, glm::vec4(1)));
	m_upWall->addComponent("showOutline", std::make_shared<ShowOutline>(*m_upWall, pShapeShader, glm::vec4(1)));
	m_plate1->addComponent("showOutline", std::make_shared<ShowOutline>(*m_plate1, pShapeShader, glm::vec4(1)));
	m_plate2->addComponent("showOutline", std::make_shared<ShowOutline>(*m_plate2, pShapeShader, glm::vec4(1)));

	m_plate1->addComponent("characterController", std::make_shared<CharacterController>(*m_plate1));
	m_plate2->addComponent("characterController", std::make_shared<CharacterController>(*m_plate2));
	m_ball->addComponent("characterController", std::make_shared<CharacterController>(*m_ball));

	auto plate1Col = std::make_shared<Physics::Collider>(*m_plate1);
	auto plate2Col = std::make_shared<Physics::Collider>(*m_plate2);
	auto downWallCol = std::make_shared<Physics::Collider>(*m_downWall);
	auto upWallCol = std::make_shared<Physics::Collider>(*m_upWall);
	auto rightWallCol = std::make_shared<Physics::Collider>(*m_rightWall);
	auto leftWallCol = std::make_shared<Physics::Collider>(*m_leftWall);
	auto ballCol = std::make_shared<Physics::Collider>(*m_ball);

	m_plate1->addComponent("collider", plate1Col);
	m_plate2->addComponent("collider", plate2Col);
	m_downWall->addComponent("collider", downWallCol);
	m_upWall->addComponent("collider", upWallCol);
	m_rightWall->addComponent("collider", rightWallCol);
	m_leftWall->addComponent("collider", leftWallCol);
	m_ball->addComponent("collider", ballCol);

	Physics::PhysicsEngine::addCollider(plate1Col);
	Physics::PhysicsEngine::addCollider(plate2Col);
	Physics::PhysicsEngine::addCollider(upWallCol);
	Physics::PhysicsEngine::addCollider(downWallCol);
	Physics::PhysicsEngine::addCollider(rightWallCol);
	Physics::PhysicsEngine::addCollider(leftWallCol);
	Physics::PhysicsEngine::addCollider(ballCol);

	plate1Col->setOnCollisionCallback(onCollisionPlate);
	plate2Col->setOnCollisionCallback(onCollisionPlate);
	ballCol->setOnCollisionCallback(onCollisionBall);
	//m_text = ResourceManager::getSprite("excPoint");

	m_ball->setMoveOffset(glm::vec2((rand() % 3) - 1, (rand() % 3) - 1));
	return true;
}
