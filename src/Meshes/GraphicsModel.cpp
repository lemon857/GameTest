#include "EngineCore/Meshes/GraphicsModel.h"

#include "EngineCore/Components/MeshRenderer.h"
#include "EngineCore/Renderer3D/GraphicsObject.h"

static int g_current_obj_ID = 0;

GraphicsModel::GraphicsModel(GraphicsObject* obj, RenderEngine::Material* pMaterial)
	: IGameObject("GraphicsModel" + std::to_string(g_current_obj_ID++))
{
	addComponent<MeshRenderer>(std::move(obj), std::move(pMaterial));
}

void GraphicsModel::reload()
{
	//std::shared_ptr<GraphicsObject> obj = ResourceManager::load_OBJ_file(m_path, true);
	//getComponent<MeshRenderer>()->update_object(std::move(obj));
}
