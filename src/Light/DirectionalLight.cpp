#include "EngineCore/Light/DirectionalLight.h"

#include "EngineCore/System/ShadersSettings.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Resources/ResourceManager.h"
#include "EngineCore/Renderer/ShaderProgram.h"

#include <glm/gtc/matrix_transform.hpp>

DirectionalLight::DirectionalLight(std::vector<std::string> shaders_names)
	: IGameObject("DirectionalLight")
	, m_light_color(glm::vec3(1.f))
	, m_shaders_names(shaders_names)
{
	addComponent<Transform>();
}

void DirectionalLight::update(const double delta)
{
	glm::vec3 rot = getComponent<Transform>()->get_rotation();

    float xRotRadians = glm::radians(rot[0]);
    float yRotRadians = glm::radians(rot[1]);
    float zRotRadians = glm::radians(rot[2]);

    glm::mat4 rotateXmat(
        1, 0, 0, 0,
        0, cos(xRotRadians), -sin(xRotRadians), 0,
        0, sin(xRotRadians), cos(xRotRadians), 0,
        0, 0, 0, 1);

    glm::mat4 rotateYmat(
        cos(yRotRadians), 0, -sin(yRotRadians), 0,
        0, 1, 0, 0,
        sin(yRotRadians), 0, cos(yRotRadians), 0,
        0, 0, 0, 1);

    glm::mat4 rotateZmat(
        cos(zRotRadians), -sin(zRotRadians), 0, 0,
        sin(zRotRadians), cos(zRotRadians), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);

	glm::vec3 finalyRot = rotateXmat * rotateYmat * rotateZmat * glm::vec4(1.f);
	for (const auto& curName : m_shaders_names)
	{
		ResourceManager::getShaderProgram(curName)->setVec3(SS_DIRECTION_LIGHT_COLOR_NAME, m_light_color);
		ResourceManager::getShaderProgram(curName)->setVec3(SS_DIRECTION_LIGHT_DIR_NAME, finalyRot);
	}
}