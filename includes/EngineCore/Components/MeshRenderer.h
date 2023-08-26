#pragma once

#include "EngineCore/IComponent.h"

#include <memory>
#include <string>

namespace RenderEngine
{
	class VertexArray;
	class IndexBuffer;
	class Material;
}

struct GraphicsObject;

class MeshRenderer : public IComponent
{
public:
	MeshRenderer(
		std::shared_ptr<GraphicsObject> obj,
		std::shared_ptr<RenderEngine::Material> pMaterial);
	~MeshRenderer();

	void update(const double delta) override;

	void update_object(std::shared_ptr<GraphicsObject> obj);

	std::shared_ptr<RenderEngine::Material> get_material_ptr();
private:
	std::shared_ptr<RenderEngine::Material> m_pMaterial;
	std::shared_ptr<GraphicsObject> m_obj;
};