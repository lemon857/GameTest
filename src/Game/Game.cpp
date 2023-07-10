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
    //ResourceManager::getAnimatedSprite("TankSprite")->render();
    m_pTank->render();
}
void Game::update(const uint64_t delta)
{
    ResourceManager::getAnimatedSprite("AnimatedSprite")->update(delta);
    //ResourceManager::getAnimatedSprite("TankSprite")->update(delta);
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
    auto pDefaultShaderProgram = ResourceManager::loadShaders("DefaultShader", "res/shaders/vertex.vert", "res/shaders/fragment.frag");
    if (!pDefaultShaderProgram)
    {
        std::cerr << "Can't create shader program!\n";
        return false;
    }

    auto pSpriteShaderProgram = ResourceManager::loadShaders("SpriteShader", "res/shaders/sprite.vert", "res/shaders/sprite.frag");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't create shader program!\n";
        return false;
    }

    auto pTex = ResourceManager::loadTexture("DefaultTexture", "res/textures/map_16x16.png");

    std::vector<std::string> subStringNames{ "eagle", "deadEagle", "respawn0", "respawn1", "respawn2", "respawn3", "respawn4", "respawn5" };
    std::vector<std::string> subStringNamesTanks{
        "YellowUp11",
        "YellowUp12",
        "YellowLeft11",
        "YellowLeft12",
        "YellowBottom11",
        "YellowBottom12",
        "YellowRight11",
        "YellowRight12"
    };
    std::vector <std::pair<std::string, uint64_t>> spawnState;

    spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn0", 50000000));
    spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn1", 50000000));
    spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn2", 50000000));
    spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn3", 50000000));
    spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn4", 80000000));
    spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn5", 100000000));
    spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn4", 100000000));
    spawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn5", 100000000));
    spawnState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 300000000));

    auto pTexAtlas = ResourceManager::loadTextureAtlas("DefaultTextureAtlas", subStringNames, "res/textures/map_16x16.png", 16, 16);
    auto pTexAtlasTank = ResourceManager::loadTextureAtlas("TanksTextureAtlas", subStringNamesTanks, "res/textures/tanks.png", 16, 16);

    auto pAnimatedSprite = ResourceManager::loadAnimatedSprite("AnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, 0, "deadEagle");
    pAnimatedSprite->setPosition(glm::vec2(240, 350));

    auto pTankSprite = ResourceManager::loadAnimatedSprite("TankSprite", "TanksTextureAtlas", "SpriteShader", 100, 100, 0, "YellowUp11");

    pAnimatedSprite->insertState("spawnState", std::move(spawnState));
    pAnimatedSprite->setState("spawnState");

    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)m_WindowSize.x, 0.0f, (float)m_WindowSize.y, -100.0f, 100.0f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

    std::vector <std::pair<std::string, uint64_t>> tankTopState;
    tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("YellowUp11", 70000000));
    tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("YellowUp12", 70000000));

    std::vector <std::pair<std::string, uint64_t>> tankBottomState;
    tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("YellowBottom11", 70000000));
    tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("YellowBottom12", 70000000));

    std::vector <std::pair<std::string, uint64_t>> tankLeftState;
    tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("YellowLeft11", 70000000));
    tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("YellowLeft12", 70000000));

    std::vector <std::pair<std::string, uint64_t>> tankRightState;
    tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("YellowRight11", 70000000));
    tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("YellowRight12", 70000000));

    pTankSprite->insertState("TopState", std::move(tankTopState));
    pTankSprite->insertState("BottomState", std::move(tankBottomState));
    pTankSprite->insertState("LeftState", std::move(tankLeftState));
    pTankSprite->insertState("RightState", std::move(tankRightState));

    pTankSprite->setState("TopState");

    m_pTank = std::make_unique<Tank>(pTankSprite, 0.0000001f, glm::vec2(100.f, 100.f));

	return true;
}