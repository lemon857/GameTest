#include "EngineCore/Meshes/ObjModel.h"

#include "EngineCore/Components/MeshRenderer.h"

static int g_current_obj_ID = 0;

ObjModel::ObjModel(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Material> pMaterial)
	: IGameObject("ObjModel" + std::to_string(g_current_obj_ID++))
{
	addComponent<MeshRenderer>(std::move(obj), std::move(pMaterial));
}

void ObjModel::reload()
{
	//std::shared_ptr<GraphicsObject> obj = ResourceManager::load_OBJ_file(m_path, true);
	//getComponent<MeshRenderer>()->update_object(std::move(obj));
}
