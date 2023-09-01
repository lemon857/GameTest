#include "EngineCore/Meshes/Plane.h"

#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/IndexBuffer.h"
#include "EngineCore/Renderer/Texture2D.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Components/MeshRenderer.h"
#include "EngineCore/Renderer3D/GraphicsObject.h"

#include <glad/glad.h>


static constexpr GLfloat m_vertexCoords[]{
     -1.f,  0.f,  -1.f,
     -1.f,  0.f,  1.f,
      1.f,  0.f,  1.f,
      1.f,  0.f,  -1.f,
};

static constexpr GLfloat m_normalCoords[]{
    0.f, 1.f,  0.f,
    0.f, 1.f,  0.f,
    0.f, 1.f,  0.f,
    0.f, 1.f,  0.f,
};

static constexpr GLfloat m_textureCoords[]{
    1.f, 0.f,
    1.f, 1.f,
    0.f, 1.f,
    0.f, 0.f,
};

static constexpr GLuint m_indexes[]{
    0, 1, 2, 2, 3, 0,
};

static int g_current_plane_ID = 0;

Plane::Plane(std::shared_ptr<RenderEngine::Material> pMaterial)
    : IGameObject("Plane" + std::to_string(g_current_plane_ID++))
    , m_pMaterial(std::move(pMaterial))
{
    std::shared_ptr<RenderEngine::VertexArray> vertexArray = std::make_shared<RenderEngine::VertexArray>();
    std::shared_ptr<RenderEngine::IndexBuffer> indexBuffer = std::make_shared<RenderEngine::IndexBuffer>();
    RenderEngine::VertexBuffer vertexCoordsBuffer;
    RenderEngine::VertexBuffer vertexNormalBuffer;
    RenderEngine::VertexBuffer textureCoordsBuffer;

    vertexCoordsBuffer.init(&m_vertexCoords, 4 * 3 * sizeof(GLfloat), false);
    RenderEngine::VertexBufferLayout vertexCoordsLayout;
    vertexCoordsLayout.addElementLayoutFloat(3, false);
    vertexArray->addBuffer(vertexCoordsBuffer, vertexCoordsLayout);

    vertexNormalBuffer.init(&m_normalCoords, 4 * 3 * sizeof(GLfloat), false);
    RenderEngine::VertexBufferLayout vertexNormalLayout;
    vertexNormalLayout.addElementLayoutFloat(3, false);
    vertexArray->addBuffer(vertexNormalBuffer, vertexNormalLayout);

    textureCoordsBuffer.init(m_textureCoords, 4 * 2 * sizeof(GLfloat), true);
    RenderEngine::VertexBufferLayout textureCoordsLayout;
    textureCoordsLayout.addElementLayoutFloat(2, false);
    vertexArray->addBuffer(textureCoordsBuffer, textureCoordsLayout);

    indexBuffer->init(&m_indexes, sizeof(m_indexes) / sizeof(GLuint));

    vertexArray->unbind();
    indexBuffer->unbind();

    addComponent<MeshRenderer>(std::make_shared<GraphicsObject>(vertexArray, indexBuffer), m_pMaterial);
}