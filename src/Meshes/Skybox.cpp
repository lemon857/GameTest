#include "EngineCore/Meshes/Skybox.h"

#include "EngineCore/Meshes/EmptyObject.h"
#include "EngineCore/Components/SpriteRenderer.h"
#include "EngineCore/Components/Transform.h"

Skybox::Skybox(glm::vec3 start_pos, glm::vec3 start_scale, std::shared_ptr<RenderEngine::Material> pMaterial)
{
	addComponent<SpriteRenderer>(pMaterial);
	EmptyObject* p1 = new EmptyObject;
	p1->addComponent<SpriteRenderer>(pMaterial, "plane0");
	p1->addComponent<Transform>(start_pos + glm::vec3(0.f, start_scale.y, 0.f), start_scale, glm::vec3(0.f, 90.f, 0.f));
	m_sprites.push_back(p1);
}

Skybox::~Skybox()
{
	for (auto& i : m_sprites)
	{
		delete i;
	}
}

void Skybox::render()
{
	for (auto& i : m_sprites)
	{
		i->render();
	}
}
