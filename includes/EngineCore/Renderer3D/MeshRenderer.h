#pragma once

#include "EngineCore/IComponent.h"

#include <memory>

namespace RenderEngine
{
	class VertexArray;
	class IndexBuffer;
	class ShaderProgram;
}

class MeshRenderer : public IComponent
{
public:
	MeshRenderer();
	~MeshRenderer();

	void init(
		std::shared_ptr<RenderEngine::VertexArray> vao,
		std::shared_ptr<RenderEngine::IndexBuffer> ebo,
		std::shared_ptr<RenderEngine::ShaderProgram> pShaderProgram);

	void update(const double delta) override;
private:
	std::shared_ptr<RenderEngine::ShaderProgram> m_pShaderProgram;
	std::shared_ptr<RenderEngine::VertexArray> m_vertexArray;
	std::shared_ptr<RenderEngine::IndexBuffer> m_indexBuffer;
};