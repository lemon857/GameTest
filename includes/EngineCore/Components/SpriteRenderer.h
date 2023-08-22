#pragma once

#include <memory>
#include <string>

#include "EngineCore/IComponent.h"

namespace RenderEngine
{
	class Texture2D;
	class ShaderProgram;
	class VertexBuffer;
	class VertexArray;
	class IndexBuffer;
	class ShaderProgram;
}

class SpriteRenderer : public IComponent
{
public:
	SpriteRenderer(std::shared_ptr<RenderEngine::Texture2D> pTexture,
		std::string initialSubTexture,
		std::shared_ptr<RenderEngine::ShaderProgram> pShaderProgram);

	~SpriteRenderer();

	void setSubTexture(std::string subTexture);

	void update(const double delta) override;
private:
	std::shared_ptr<RenderEngine::Texture2D> m_pTextureAtlas;
	std::shared_ptr<RenderEngine::ShaderProgram> m_pShaderProgram;

	std::shared_ptr<RenderEngine::VertexArray> m_vertexArray;
	RenderEngine::VertexBuffer* m_vertexCoordsBuffer;
	RenderEngine::VertexBuffer* m_textureCoordsBuffer;
	RenderEngine::IndexBuffer* m_indexBuffer;
};