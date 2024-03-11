#pragma once

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
	SpriteRenderer(RenderEngine::Material* pMaterial,
		std::string initialSubTexture = "default");

	~SpriteRenderer();

	void setSubTexture(std::string subTexture);

	void render(RenderEngine::ShaderProgram* shader) override;

	RenderEngine::Material* get_material_ptr();
private:
	RenderEngine::Material* m_pMaterial;

	RenderEngine::VertexArray* m_vertexArray;
	RenderEngine::VertexBuffer* m_vertexCoordsBuffer;
	RenderEngine::VertexBuffer* m_textureCoordsBuffer;
	RenderEngine::IndexBuffer* m_indexBuffer;
};