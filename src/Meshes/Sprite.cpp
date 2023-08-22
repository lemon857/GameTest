#include "EngineCore/Meshes/Sprite.h"

#include "EngineCore/Components/SpriteRenderer.h"

static int g_current_sprite_ID = 0;

Sprite::Sprite(std::shared_ptr<RenderEngine::Texture2D> pTexture, std::string initSubTexture, std::shared_ptr<RenderEngine::ShaderProgram> pShader)
	: IGameObject("Sprite" + std::to_string(g_current_sprite_ID++))
{
	addComponent<SpriteRenderer>(pTexture, initSubTexture, pShader);
}

void Sprite::setSubTexture(std::string subTexture)
{
	getComponent<SpriteRenderer>()->setSubTexture(subTexture);
}
