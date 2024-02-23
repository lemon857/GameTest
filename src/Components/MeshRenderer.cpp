#include "EngineCore/Components/MeshRenderer.h"

#include "EngineCore/IGameObject.h"
#include "EngineCore/Components/Transform.h"
#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/IndexBuffer.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer3D/GraphicsObject.h"

#include "EngineCore/System/ShadersSettings.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

MeshRenderer::MeshRenderer(
    std::shared_ptr<GraphicsObject> obj,
    std::shared_ptr<RenderEngine::Material> pMaterial)
    : IComponent()
    , m_obj(std::move(obj))
    , m_pMaterial(std::move(pMaterial))
{
}

MeshRenderer::~MeshRenderer()
{
    if (m_obj == nullptr) return;
	delete m_obj.get();
    m_obj.reset();
}

void MeshRenderer::render(RenderEngine::ShaderProgram* shader)
{
    if (m_obj == nullptr) return;
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

    m_pMaterial->use();
    m_pMaterial->set_model_matrix(model);

    if (shader != nullptr)
    {
        float add = 1.1f;
        glm::mat4 scaleMat1(
            scale[0] * add, 0, 0, 0,
            0, scale[1] * add, 0, 0,
            0, 0, scale[2] * add, 0,
            0, 0, 0, 1);
        glm::mat4 model1 = translateMat * rotateXmat * rotateYmat * rotateZmat * scaleMat1;

        shader->use();
        shader->setMatrix4(SS_MODEL_MATRIX_NAME, model1);
    }

    RenderEngine::Renderer::drawTriangles(*m_obj->vertex_array, *m_obj->index_buffer);
}

void MeshRenderer::update_object(std::shared_ptr<GraphicsObject> obj)
{
    m_obj = std::move(obj);
}

std::shared_ptr<RenderEngine::Material> MeshRenderer::get_material_ptr()
{
    return m_pMaterial;
}

const std::shared_ptr<GraphicsObject> MeshRenderer::get_object()
{
    return m_obj;
}
