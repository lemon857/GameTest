#pragma once
//Интерфейс для создания класса игры

#include "Engine.h"
#include "../Renderer/Renderer.h"
#include <glm/vec2.hpp>
#include <array>

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
    
    void setWindowSize(const glm::ivec2& windowSize) { m_WindowSize = windowSize; };
    void setKey(const int key, const int action) { m_keys[key] = action; }
protected:
    glm::ivec2 m_WindowSize;
    std::array<bool, 349> m_keys;
  
};