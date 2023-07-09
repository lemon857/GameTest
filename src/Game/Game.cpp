#include "Game.h"

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
    ResourceManager::getAnimatedSprite("TankSprite")->render();
    ResourceManager::getAnimatedSprite("AnimatedSprite")->render();
}
void Game::update(const uint64_t delta)
{
    ResourceManager::getAnimatedSprite("TankSprite")->update(delta);
    ResourceManager::getAnimatedSprite("AnimatedSprite")->update(delta);
}
void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}
bool Game::init()
{
    auto pDefaultShaderProgram = ResourceManager::loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
    if (!pDefaultShaderProgram)
    {
        std::cerr << "Can't create shader program!\n";
        return false;
    }

    auto pSpriteShaderProgram = ResourceManager::loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
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
        "YellowDown11",
        "YellowDown12",
        "YellowRight11",
        "YellowRight12"
    };
    std::vector <std::pair<std::string, uint64_t>> spawnState;

    std::vector <std::pair<std::string, uint64_t>> tankState;


    tankState.emplace_back(std::make_pair<std::string, uint64_t>("YellowUp11", 70000000));
    tankState.emplace_back(std::make_pair<std::string, uint64_t>("YellowUp12", 70000000));

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
    pTankSprite->setPosition(glm::vec2(240, 120));

    pAnimatedSprite->insertState("waterState", std::move(spawnState));
    pAnimatedSprite->setState("waterState");

    pTankSprite->insertState("up1", std::move(tankState));
    pTankSprite->setState("up1");

    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)m_WindowSize.x, 0.0f, (float)m_WindowSize.y, -100.0f, 100.0f);

    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);
    pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

	return true;
}