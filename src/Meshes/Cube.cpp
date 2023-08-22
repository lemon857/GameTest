#include "EngineCore/Meshes/Cube.h"

#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/IndexBuffer.h"
#include "EngineCore/Components/MeshRenderer.h"

static int g_current_ID = 0;

Cube::Cube(std::shared_ptr<RenderEngine::ShaderProgram> pShader, std::shared_ptr<RenderEngine::Texture2D> pTexture)
	: IGameObject("Cube" + std::to_string(g_current_ID++))
{
    std::shared_ptr<RenderEngine::VertexArray> vertexArray = std::make_shared<RenderEngine::VertexArray>();
    std::shared_ptr<RenderEngine::IndexBuffer> indexBuffer = std::make_shared<RenderEngine::IndexBuffer>();
    RenderEngine::VertexBuffer m_vertexCoordsBuffer;
    RenderEngine::VertexBuffer m_vertexNormalBuffer;
    RenderEngine::VertexBuffer m_textureCoordsBuffer;

    m_vertexCoordsBuffer.init(&m_vertexCoords, 24 * 3 * sizeof(GLfloat));
    RenderEngine::VertexBufferLayout vertexCoordsLayout;
    vertexCoordsLayout.addElementLayoutFloat(3, false);
    vertexArray->addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

    m_vertexNormalBuffer.init(&m_normalCoords, 24 * 3 * sizeof(GLfloat));
    RenderEngine::VertexBufferLayout vertexNormalLayout;
    vertexNormalLayout.addElementLayoutFloat(3, false);
    vertexArray->addBuffer(m_vertexNormalBuffer, vertexNormalLayout);

    m_textureCoordsBuffer.init(&m_textureCoords, 24 * 2 * sizeof(GLfloat));
    RenderEngine::VertexBufferLayout textureCoordsLayout;
    textureCoordsLayout.addElementLayoutFloat(2, false);
    vertexArray->addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

    indexBuffer->init(&m_indexes, sizeof(m_indexes) / sizeof(GLuint));

    vertexArray->unbind();
	indexBuffer->unbind();

    addComponent<MeshRenderer>()->init(vertexArray, indexBuffer, pShader, pTexture);
}
