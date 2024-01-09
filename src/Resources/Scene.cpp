#include "EngineCore/Resources/Scene.h"

void Scene::clear()
{
	for (size_t i = 0; i < m_objs.size(); i++)
	{
		delete m_objs[i];
	}
	m_objs.clear();
}
