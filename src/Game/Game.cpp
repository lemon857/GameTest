#include "Game.h"
#include "Tank.h"

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
    ResourceManager::getSprite("wall")->render(glm::vec2(100.f, 200.f), glm::vec2(50.f, 50.f), 0, 0);
    ResourceManager::getSprite("wall")->render(glm::vec2(150.f, 200.f), glm::vec2(50.f, 50.f), 0, 0);
    ResourceManager::getSprite("wall")->render(glm::vec2(150.f, 250.f), glm::vec2(50.f, 50.f), 0, 0);
    ResourceManager::getSprite("wall")->render(glm::vec2(100.f, 250.f), glm::vec2(50.f, 50.f), 0, 0);
    m_pTank->render();
}
void Game::update(const double delta)
{
    m_pAnimator->update(delta);
    if (m_pTank)
    {
        if (m_keys[GLFW_KEY_W])
        {
            m_pTank->setOrentation(Tank::EOrentation::Top);
            m_pTank->move(true);
            m_pAnimator->startAnimation("TopMove");
        }
        else if ((m_keys[GLFW_KEY_A]))
        {
            m_pTank->setOrentation(Tank::EOrentation::Left);
            m_pTank->move(true);
            m_pAnimator->startAnimation("LeftMove");
        }
        else if ((m_keys[GLFW_KEY_S]))
        {
            m_pTank->setOrentation(Tank::EOrentation::Bottom);
            m_pTank->move(true);
            m_pAnimator->startAnimation("BottomMove");
        }
        else if ((m_keys[GLFW_KEY_D]))
        {
            m_pTank->setOrentation(Tank::EOrentation::Right);
            m_pTank->move(true);
            m_pAnimator->startAnimation("RightMove");
        }
        else
        {
            m_pTank->move(false);
        }
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

    auto pTankSprite = ResourceManager::getSprite("TankSprite");
    ResourceManager::loadSprite("wall", "WallAtlas", "spriteShader", "BrickWall");

    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)m_WindowSize.x, 0.0f, (float)m_WindowSize.y, -100.0f, 100.0f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    m_pAnimator = ResourceManager::getAnimator("moveAnimator");

    m_pTank = std::make_shared<Tank>(pTankSprite, 0.5, 0.05, glm::vec2(100.f, 200.f), glm::vec2(125.f, 125.f));
    //m_pTank2 = std::make_shared<Tank>(pTankSprite, 0.5, 1, glm::vec2(100.f, 0.f), glm::vec2(100.f, 100.f));

    m_pTank->setKinematicState(false);
    //m_pTank2->setKinematicState(true);

    Physics::PhysicsEngine::addDynamicObj(m_pTank, 1);
    //Physics::PhysicsEngine::addDynamicObj(m_pTank2, 2);
    
    //std::shared_ptr<Physics::Collider>col1 = std::make_shared<Physics::Collider>(m_pTank);
    //std::shared_ptr<Physics::Collider>col2 = std::make_shared<Physics::Collider>(m_pTank2);

    //Physics::PhysicsEngine::addCollider(col1, 1);
    //Physics::PhysicsEngine::addCollider(col2, 2);

	return true;
}