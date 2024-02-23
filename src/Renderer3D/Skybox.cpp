#include "EngineCore/Renderer3D/Skybox.h"
#include "EngineCore/Renderer3D/Texture3D.h"
#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/System/ShadersSettings.h"

#include <glad/glad.h>

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

RenderEngine::Skybox::Skybox(std::vector<std::string> faces, ShaderProgram* shader)
	: m_shader(std::move(shader))
{
	unsigned int skyboxVBO;
	glGenVertexArrays(1, &m_cube);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(m_cube);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	m_texture = new Texture3D(faces);

	m_shader->use();
	m_shader->setInt("skybox", 0);
}

RenderEngine::Skybox::Skybox(Texture3D* texture, ShaderProgram* shader)
	: m_shader(std::move(shader))
	, m_texture(std::move(texture))
{
	unsigned int skyboxVBO;
	glGenVertexArrays(1, &m_cube);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(m_cube);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	m_shader->use();
	m_shader->setInt("skybox", 0);
}

RenderEngine::Skybox::~Skybox()
{
	delete m_texture;
}

void RenderEngine::Skybox::render(glm::mat4& prj)
{
	glDepthFunc(GL_LEQUAL);
	m_shader->use();
	m_shader->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, prj);
	glBindVertexArray(m_cube);
	glActiveTexture(GL_TEXTURE0);
	m_texture->bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}
