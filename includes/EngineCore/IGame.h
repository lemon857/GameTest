#pragma once
//Интерфейс для создания класса игры

#include "Engine.h"

#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/Camera.h"

#include <glm/vec2.hpp>
#include <array>
#include <memory>

class IGame
{
public:
    IGame(const glm::ivec2& windowSize)
        : m_WindowSize(windowSize)
    {
        m_keys.fill(false);
    };

    virtual void update(const double delta) {};
    virtual void render() const {};
    virtual bool init() { return false; };
    
    void setWindowSize(const glm::ivec2& windowSize) { m_WindowSize = windowSize; /*m_cam->updateSize((glm::vec2)windowSize);*/ };
    void setKey(const int key, const int action) { m_keys[key] = action; };
protected:
    glm::ivec2 m_WindowSize;
    std::array<int, 349> m_keys;
    std::shared_ptr<Camera> m_cam;
    
};