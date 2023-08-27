#include "EngineCore/Meshes/Sprite.h"

#include "EngineCore/Components/SpriteRenderer.h"

static int g_current_sprite_ID = 0;

Sprite::Sprite(std::shared_ptr<RenderEngine::Material> pMaterial, std::string initSubTexture)
	: IGameObject("Sprite" + std::to_string(g_current_sprite_ID++))
{
	addComponent<SpriteRenderer>(pMaterial, initSubTexture);
}

void Sprite::setSubTexture(std::string subTexture)
{
	getComponent<SpriteRenderer>()->setSubTexture(subTexture);
}
