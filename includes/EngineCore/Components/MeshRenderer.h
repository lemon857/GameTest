#pragma once

#include "EngineCore/IComponent.h"

#include <string>

namespace RenderEngine
{
	class VertexArray;
	class IndexBuffer;
	class Material;
	class ShaderProgram;
}

struct GraphicsObject;

class MeshRenderer : public IComponent
{
public:
	MeshRenderer(GraphicsObject* obj, RenderEngine::Material* pMaterial);
	~MeshRenderer();

	void render(RenderEngine::ShaderProgram* shader) override;

	void update_object(GraphicsObject* obj);

	RenderEngine::Material* get_material_ptr();

	const GraphicsObject* get_object();
private:
	RenderEngine::Material* m_pMaterial;
	GraphicsObject* m_obj;
};