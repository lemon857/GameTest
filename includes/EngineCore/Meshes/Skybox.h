#pragma once

#include "EngineCore/IGameObject.h"

#include <glm/vec3.hpp>
#include <memory>
#include <vector>

class SpriteRenderer;
class EmptyObject;

namespace RenderEngine
{
	class Material;
}
// need input texture with 6 subTextures: "plane" + from 0 to 5
class Skybox : public IGameObject
{
public:
	Skybox(glm::vec3 start_pos, glm::vec3 start_scale, std::shared_ptr<RenderEngine::Material> pMaterial);
	~Skybox();

	void render() override;

private:
	std::vector<EmptyObject*> m_sprites;
};