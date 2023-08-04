#include "Game.h"
#include "Tank.h"
#include "BrickWall.h"

#include<string>
#include<iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/Animator.h"
#include "../Resources/ResourceManager.h"
#include "../Physics/PhysicsEngine.h"
#include "../Physics/Collider.h"

Game::Game(const glm::ivec2& windowSize)
    : m_eCurrentGameState(EGameState::Active)
    , m_WindowSize(windowSize)
{
	m_keys.fill(false);
}
Game::~Game()
{

}

void Game::render()
{
    m_pTank->render();
    m_pBrickWall->render();
    m_point->render(glm::vec2(50.f, 50.f), 2, glm::vec4(1));
    m_line->render(glm::vec2(50.f, 50.f), glm::vec2(50.f, 50.f), 2, glm::vec4(1));
}
void Game::update(const double delta)
{
    if (m_pTank)
    {
        if (m_keys[GLFW_KEY_W])
        {
            m_pTank->setOrentation(Physics::EDirection::Up);
            m_pTank->move(true);
        }
        else if ((m_keys[GLFW_KEY_A]))
        {
            m_pTank->setOrentation(Physics::EDirection::Left);
            m_pTank->move(true);
        }
        else if ((m_keys[GLFW_KEY_S]))
        {
            m_pTank->setOrentation(Physics::EDirection::Down);
            m_pTank->move(true);
        }
        else if ((m_keys[GLFW_KEY_D]))
        {
            m_pTank->setOrentation(Physics::EDirection::Right);
            m_pTank->move(true);
        }
        else
        {
            m_pTank->move(false);
        }
        m_pTank->update(delta);
    }
}
void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}
bool Game::init()
{
    ResourceManager::loadJSONresources("res/resources.json");

    auto pSpriteShaderProgram = ResourceManager::getShaderProgram("spriteShader");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't create shader program!\n";
        return false;
    }

    auto pShapeShaderProgram = ResourceManager::getShaderProgram("shapeShader");
    if (!pShapeShaderProgram)
    {
        std::cerr << "Can't create shader program!\n";
        return false;
    }

    m_point = std::make_shared<RenderEngine::Point>(pShapeShaderProgram);
    m_line = std::make_shared<RenderEngine::Line>(pShapeShaderProgram);

    auto pTankSprite = ResourceManager::getSprite("TankSprite");
    auto pWallSprite = ResourceManager::getSprite("BrickWallSprite");

    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)m_WindowSize.x, 0.0f, (float)m_WindowSize.y, -100.0f, 100.0f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    pShapeShaderProgram->use();
    pShapeShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    m_pTank = std::make_shared<Tank>(pTankSprite, 0.5, 0.05, glm::vec2(0.f, 0.f), glm::vec2(100.f, 100.f));
    m_pBrickWall = std::make_shared<BrickWall>(pWallSprite, glm::vec2(200.f, 100.f), glm::vec2(160.f, 160.f));
    //m_pTank2 = std::make_shared<Tank>(pTankSprite, 0.5, 1, glm::vec2(100.f, 0.f), glm::vec2(100.f, 100.f));

    //m_pTank->setKinematicState(false);
    //m_pTank2->setKinematicState(true);

    //Physics::PhysicsEngine::addDynamicObj(m_pTank, 1);
    //Physics::PhysicsEngine::addDynamicObj(m_pTank2, 2);
    //Physics::PhysicsEngine::addImpulse(glm::vec2(10, 0), 1);
    //std::shared_ptr<Physics::Collider>col1 = std::make_shared<Physics::Collider>(m_pTank);
    //std::shared_ptr<Physics::Collider>col2 = std::make_shared<Physics::Collider>(m_pTank2);

    //Physics::PhysicsEngine::addCollider(col1, 1);
    //Physics::PhysicsEngine::addCollider(col2, 2);

	return true;
}