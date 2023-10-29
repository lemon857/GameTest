#include "EngineCore/Light/PointerLight.h"

#include "EngineCore/System/ShadersSettings.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/ShaderProgram.h"

#include <glm/gtc/matrix_transform.hpp>

PointerLight::PointerLight(std::vector<std::string> shaders_names, float strength_light)
	: IGameObject("PointerLight")
	, m_light_color(glm::vec3(1.f))
	, m_shaders_names(shaders_names)
	, m_strength_light(strength_light)
{
	addComponent<Transform>();
}

void PointerLight::update(const double delta)
{
	glm::vec3 pos = getComponent<Transform>()->get_position();
	for (const auto& curName : m_shaders_names)
	{
		ResourceManager::getShaderProgram(curName)->setVec3(SS_POINTER_LIGHT_COLOR_NAME, m_light_color);
		ResourceManager::getShaderProgram(curName)->setVec3(SS_POINTER_LIGHT_POS_NAME, pos);
		ResourceManager::getShaderProgram(curName)->setFloat(SS_POINTER_LIGHT_STRENGTH_NAME, m_strength_light);
	}
}