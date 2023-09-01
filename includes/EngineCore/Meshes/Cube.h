#pragma once

#include "EngineCore/IGameObject.h"

#include <memory>
#include <string>

namespace RenderEngine
{
	class Material;
}

class Cube : public IGameObject
{
public:
	Cube(std::shared_ptr<RenderEngine::Material> pMaterial);

    ~Cube() = default;

    Cube(Cube&) = delete;
    Cube(Cube&&) = delete;

    Cube& operator=(Cube&) = delete;
    Cube& operator=(Cube&&) = delete;
private:
    std::shared_ptr<RenderEngine::Material> m_pMaterial;
};