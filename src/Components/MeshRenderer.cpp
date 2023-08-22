#include "EngineCore/Components/MeshRenderer.h"

#include "EngineCore/IGameObject.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/IndexBuffer.h"
#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/Texture2D.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

MeshRenderer::MeshRenderer(
    std::shared_ptr<RenderEngine::VertexArray> vao,
    std::shared_ptr<RenderEngine::IndexBuffer> ebo,
    std::shared_ptr<RenderEngine::ShaderProgram> pShaderProgram,
    std::shared_ptr<RenderEngine::Texture2D> pTexture)
    : IComponent()
    , m_vertexArray(std::move(vao))
    , m_indexBuffer(std::move(ebo))
    , m_pShaderProgram(std::move(pShaderProgram))
    , m_pTexture(std::move(pTexture))
{
}

MeshRenderer::~MeshRenderer()
{
	delete m_vertexArray.get();
	delete m_indexBuffer.get();
    m_vertexArray.reset();
    m_indexBuffer.reset();
}

void MeshRenderer::update(double delta)
{
	Transform* transform = m_targetObj->getComponent<Transform>();
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 scale;
    if (transform == nullptr)
    {
        scale = glm::vec3(1.f);;
        pos = glm::vec3(0.f);;
        rot = glm::vec3(0.f);
    }
    else
    {
        scale = transform->get_scale();
        pos = transform->get_position();
        rot = transform->get_rotation();
    }

    m_pShaderProgram->use();

    glm::mat4 scaleMat(
        scale[0], 0, 0, 0,
        0, scale[1], 0, 0,
        0, 0, scale[2], 0,
        0, 0, 0, 1);

    glm::mat4 translateMat(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        pos[0], pos[1], pos[2], 1);

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

    glm::mat4 model = translateMat * rotateXmat * rotateYmat * rotateZmat * scaleMat;

    m_pShaderProgram->setMatrix4("modelMat", model);

    RenderEngine::Renderer::bindTexture(*m_pTexture);
    RenderEngine::Renderer::drawTriangles(*m_vertexArray, *m_indexBuffer);
}
