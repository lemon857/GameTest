#pragma once

#include "EngineCore/IComponent.h"

#include <memory>
#include <string>

namespace RenderEngine
{
	class VertexArray;
	class IndexBuffer;
	class ShaderProgram;
	class Texture2D;
}

class MeshRenderer : public IComponent
{
public:
	MeshRenderer(
		std::shared_ptr<RenderEngine::VertexArray> vao,
		std::shared_ptr<RenderEngine::IndexBuffer> ebo,
		std::shared_ptr<RenderEngine::ShaderProgram> pShaderProgram,
		std::shared_ptr<RenderEngine::Texture2D> pTexture);
	~MeshRenderer();

	void update(const double delta) override;
private:
	std::shared_ptr<RenderEngine::ShaderProgram> m_pShaderProgram;
	std::shared_ptr<RenderEngine::Texture2D> m_pTexture;
	std::shared_ptr<RenderEngine::VertexArray> m_vertexArray;
	std::shared_ptr<RenderEngine::IndexBuffer> m_indexBuffer;
};