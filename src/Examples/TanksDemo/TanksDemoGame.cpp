#include "TanksDemoGame.h"
#include "Tank.h"
#include "BrickWall.h"

#include<string>
#include<iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/Texture2D.h"
#include "EngineCore/Renderer/Sprite.h"
#include "EngineCore/Renderer/Animator.h"
#include "EngineCore/Renderer/Animation.h"
#include "EngineCore/Renderer/ShowOutline.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Physics/PhysicsEngine.h"
#include "EngineCore/Physics/Collider.h"
#include "EngineCore/Physics/MoveController.h"

TanksDemoGame::TanksDemoGame(const glm::ivec2& windowSize)
    : IGame(windowSize)
{

}
TanksDemoGame::~TanksDemoGame()
{

}
void TanksDemoGame::render() const
{
    m_pTank->render();
    m_pBrickWall->render();
    m_pBrickWall2->render();
}
void TanksDemoGame::update(const double delta)
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
    if (m_keys[GLFW_KEY_F])
    {
        m_pTank->fire();
    }
    m_pTank->update(delta);
    //m_cam->setPosition(m_pTank->getPosition() + (m_pTank->getSize() / 2.f));
    //m_cam->update(delta);
    m_pBrickWall->update(delta);

}
bool TanksDemoGame::init()
{
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

    m_line = std::make_shared<RenderEngine::Line>(pShapeShaderProgram);

    auto pTankSprite = ResourceManager::getSprite("TankSprite");
    auto pBulletSprite = ResourceManager::getSprite("BulletSprite");
    auto pWallSprite = ResourceManager::getSprite("BrickWallSprite");

    //m_cam = std::make_shared<Camera>(glm::vec2(0), m_WindowSize, -100.f, 100.f);

    //m_cam->addShaderProgram(pSpriteShaderProgram);
    //m_cam->addShaderProgram(pShapeShaderProgram);

    //glm::perspective(90.f, 1.f, -100.f, 100.f);
    pSpriteShaderProgram->use();
    pSpriteShaderProgram->setInt("tex", 0);

    m_pTank = std::make_shared<Tank>(pTankSprite, pBulletSprite, 0.2, 0.05, glm::vec2(100.f, 0.f), glm::vec2(100.f, 100.f));
    m_pBrickWall = std::make_shared<BrickWall>(pWallSprite, glm::vec2(200.f, 100.f), glm::vec2(100.f, 100.f));
    m_pBrickWall2 = std::make_shared<BrickWall>(pWallSprite, glm::vec2(465.f, 100.f), glm::vec2(160.f, 160.f));
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

    std::shared_ptr<RenderEngine::Animator> m_pAnimator = std::make_shared<RenderEngine::Animator>(*m_pTank);

    std::vector<double> durations = { 100,	100 };

    std::vector<std::string> subTextureNames = { "YellowUp11",	"YellowUp12" };
    m_pAnimator->addAnimation("TopMove", std::make_shared<RenderEngine::Animation>(subTextureNames, durations,
        RenderEngine::ECallbackAction(0), RenderEngine::ETypeAnimation(1)));

    subTextureNames = { "YellowDown11",	"YellowDown12" };
    m_pAnimator->addAnimation("BottomMove", std::make_shared<RenderEngine::Animation>(subTextureNames, durations,
        RenderEngine::ECallbackAction(0), RenderEngine::ETypeAnimation(1)));

    subTextureNames = { "YellowLeft11",	"YellowLeft12" };
    m_pAnimator->addAnimation("LeftMove", std::make_shared<RenderEngine::Animation>(subTextureNames, durations,
        RenderEngine::ECallbackAction(0), RenderEngine::ETypeAnimation(1)));

    subTextureNames = { "YellowRight11",	"YellowRight12" };
    m_pAnimator->addAnimation("RightMove", std::make_shared<RenderEngine::Animation>(subTextureNames, durations,
        RenderEngine::ECallbackAction(0), RenderEngine::ETypeAnimation(1)));

    //auto tankCol = std::make_shared<Physics::Collider>();
    //auto wallCol = std::make_shared<Physics::Collider>();
    //auto wallCol2 = std::make_shared<Physics::Collider>();

   /* tankCol->setOnCollisionCallback(
        [](IGameObject& targetObj, IGameObject& obj, Physics::EDirection dir)
        {
            /*if (obj.getName() == "wall")
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
        });*/

    /*m_pTank->addComponent("collider", tankCol);
    m_pTank->addComponent("MoveController", std::make_shared<MoveController>(*m_pTank));
    m_pTank->addComponent("showOutline", std::make_shared<ShowOutline>(*m_pTank, pShapeShaderProgram, glm::vec4(1)));
    m_pTank->addComponent("animator", m_pAnimator);
    m_pBrickWall->addComponent("collider", wallCol);
    m_pBrickWall2->addComponent("collider", wallCol2);
    m_pBrickWall->addComponent("showOutline", std::make_shared<ShowOutline>(*m_pBrickWall, pShapeShaderProgram, glm::vec4(1)));*/

    //Physics::PhysicsEngine::addCollider(tankCol);
    //Physics::PhysicsEngine::addCollider(wallCol);
    //Physics::PhysicsEngine::addCollider(wallCol2);

	return true;
}