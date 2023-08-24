#include "EngineCore/Meshes/ObjModel.h"

#include "EngineCore/Components/MeshRenderer.h"
#include "EngineCore/Renderer3D/GraphicsObject.h"

#include "EngineCore/Resources/ResourceManager.h"

static int g_current_obj_ID = 0;

ObjModel::ObjModel(const std::string relativeFilePath, std::shared_ptr<RenderEngine::Material> pMaterial)
	: IGameObject("ObjModel" + std::to_string(g_current_obj_ID++))
	, m_path(relativeFilePath)
{
	std::shared_ptr<GraphicsObject> obj = ResourceManager::load_OBJ_file(relativeFilePath);
	addComponent<MeshRenderer>(std::move(obj), std::move(pMaterial));
}

void ObjModel::reload()
{
	std::shared_ptr<GraphicsObject> obj = ResourceManager::load_OBJ_file(m_path, true);
	getComponent<MeshRenderer>()->update_object(obj);
}
