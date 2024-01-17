#include "EngineCore/Resources/Scene.h"

#include "EngineCore/Components/MeshRenderer.h"
#include "EngineCore/Renderer/Material.h"

void Scene::clear()
{
	for (size_t i = 0; i < m_objs.size(); i++)
	{
		delete m_objs[i];
		m_objs.remove(i);
	}
	m_objs.clear();
	m_items_str.clear();
}

void Scene::update(const double delta)
{
	for (size_t i = 0; i < m_objs.size(); i++)
	{
		m_objs[i]->object->update(delta);
	}
}

void Scene::render(glm::mat4& prj)
{
	for (size_t i = 0; i < m_objs.size(); i++)
	{
		auto a = m_objs[i]->object->getComponent<MeshRenderer>();
		if (a != nullptr) a->get_material_ptr()->set_view_projection_matrix(prj);
		m_objs[i]->object->render();
	}
}
