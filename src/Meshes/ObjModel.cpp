#include "EngineCore/Meshes/ObjModel.h"

#include "EngineCore/Components/MeshRenderer.h"
#include "EngineCore/Renderer3D/GraphicsObject.h"

static int g_current_obj_ID = 0;

ObjModel::ObjModel(std::shared_ptr<GraphicsObject> obj, std::shared_ptr<RenderEngine::Texture2D> pTexture, std::shared_ptr<RenderEngine::ShaderProgram> pShader)
	: IGameObject("ObjModel" + std::to_string(g_current_obj_ID++))
	, m_pTexture(std::move(pTexture))
{
	addComponent<MeshRenderer>(std::move(obj), std::move(pShader), m_pTexture);
}