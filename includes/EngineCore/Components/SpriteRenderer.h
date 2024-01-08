#pragma once

#include <memory>
#include <string>

#include "EngineCore/IComponent.h"

namespace RenderEngine
{
	class Material;
	class VertexBuffer;
	class VertexArray;
	class IndexBuffer;
	class ShaderProgram;
}

class SpriteRenderer : public IComponent
{
public:
	SpriteRenderer(std::shared_ptr<RenderEngine::Material> pMaterial,
		std::string initialSubTexture = "default");

	~SpriteRenderer();

	void setSubTexture(std::string subTexture);

	void render() override;

	std::shared_ptr<RenderEngine::Material> get_material_ptr();
private:
	std::shared_ptr<RenderEngine::Material> m_pMaterial;

	std::shared_ptr<RenderEngine::VertexArray> m_vertexArray;
	RenderEngine::VertexBuffer* m_vertexCoordsBuffer;
	RenderEngine::VertexBuffer* m_textureCoordsBuffer;
	RenderEngine::IndexBuffer* m_indexBuffer;
};