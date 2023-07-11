#include "Game.h"
#include "Tank.h"

#include<string>
#include<iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"
#include "../Resources/ResourceManager.h"

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
    ResourceManager::getAnimatedSprite("AnimatedSprite")->render();
    m_pTank->render();
}
void Game::update(const uint64_t delta)
{
    ResourceManager::getAnimatedSprite("AnimatedSprite")->update(delta);
    if (m_pTank)
    {
        if (m_keys[GLFW_KEY_W])
        {
            m_pTank->setOrentation(Tank::EOrentation::Top);
            m_pTank->move(true);
        }
        else if ((m_keys[GLFW_KEY_A]))
        {
            m_pTank->setOrentation(Tank::EOrentation::Left);
            m_pTank->move(true);
        }
        else if ((m_keys[GLFW_KEY_S]))
        {
            m_pTank->setOrentation(Tank::EOrentation::Bottom);
            m_pTank->move(true);
        }
        else if ((m_keys[GLFW_KEY_D]))
        {
            m_pTank->setOrentation(Tank::EOrentation::Right);
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

    auto pAnimatedSprite = ResourceManager::getAnimatedSprite("AnimatedSprite");
    pAnimatedSprite->setPosition(glm::vec2(240, 350));
    pAnimatedSprite->setState("spawnState");
    auto pTankSprite = ResourceManager::getAnimatedSprite("TankSprite");

    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)m_WindowSize.x, 0.0f, (float)m_WindowSize.y, -100.0f, 100.0f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    pTankSprite->setState("TopState");

    m_pTank = std::make_unique<Tank>(pTankSprite, 0.0000005f, glm::vec2(100.f, 100.f));

	return true;
}