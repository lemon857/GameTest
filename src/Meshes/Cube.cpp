#include "EngineCore/Meshes/Cube.h"

#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/IndexBuffer.h"
#include "EngineCore/Renderer/Texture2D.h"
#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Components/MeshRenderer.h"
#include "EngineCore/Renderer3D/GraphicsObject.h"

static int g_current_cube_ID = 0;

Cube::Cube(std::shared_ptr<RenderEngine::Material> pMaterial, std::string subTextureName)
    : IGameObject("Cube" + std::to_string(g_current_cube_ID++))
    , m_pMaterial(std::move(pMaterial))
{
    std::shared_ptr<RenderEngine::VertexArray> vertexArray = std::make_shared<RenderEngine::VertexArray>();
    std::shared_ptr<RenderEngine::IndexBuffer> indexBuffer = std::make_shared<RenderEngine::IndexBuffer>();
    RenderEngine::VertexBuffer vertexCoordsBuffer;
    RenderEngine::VertexBuffer vertexNormalBuffer;
    m_textureCoordsBuffer = new RenderEngine::VertexBuffer();
    GLfloat* textureCoords;
    if (m_pMaterial->get_texture_ptr() != nullptr)
    {
        auto aSubTexture = m_pMaterial->get_texture_ptr()->getSubTexture(subTextureName);
        GLfloat textures[]{
            // FRONT
            aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
            aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
            aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
            aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
            // BACK
            aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
            aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
            aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
            aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
            // RIGHT
            aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
            aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
            aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
            aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
            // LEFT
            aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
            aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
            aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
            aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
            // TOP
            aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
            aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
            aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
            aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
            // BOTTOM
            aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
            aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
            aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
            aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
        };
        textureCoords = textures;
    }
    vertexCoordsBuffer.init(&m_vertexCoords, 24 * 3 * sizeof(GLfloat), false);
    RenderEngine::VertexBufferLayout vertexCoordsLayout;
    vertexCoordsLayout.addElementLayoutFloat(3, false);
    vertexArray->addBuffer(vertexCoordsBuffer, vertexCoordsLayout);

    vertexNormalBuffer.init(&m_normalCoords, 24 * 3 * sizeof(GLfloat), false);
    RenderEngine::VertexBufferLayout vertexNormalLayout;
    vertexNormalLayout.addElementLayoutFloat(3, false);
    vertexArray->addBuffer(vertexNormalBuffer, vertexNormalLayout);

    if (m_pMaterial->get_texture_ptr() != nullptr)
    {
        m_textureCoordsBuffer->init(textureCoords, 24 * 2 * sizeof(GLfloat), true);
        RenderEngine::VertexBufferLayout textureCoordsLayout;
        textureCoordsLayout.addElementLayoutFloat(2, false);
        vertexArray->addBuffer(*m_textureCoordsBuffer, textureCoordsLayout);
    }

    indexBuffer->init(&m_indexes, sizeof(m_indexes) / sizeof(GLuint));

    vertexArray->unbind();
	indexBuffer->unbind();

    addComponent<MeshRenderer>(std::make_shared<GraphicsObject>(vertexArray, indexBuffer), m_pMaterial);
}

Cube::~Cube()
{
    delete m_textureCoordsBuffer;
}

void Cube::setSubTexture(std::string subTextureName)
{
    if (m_pMaterial->get_texture_ptr() == nullptr) return;
    auto aSubTexture = m_pMaterial->get_texture_ptr()->getSubTexture(subTextureName);
    GLfloat textureCoords[]{
        // FRONT
        aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
        aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
        aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
        aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
        // BACK
        aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
        aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
        aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
        aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
        // RIGHT
        aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
        aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
        aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
        aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
        // LEFT
        aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
        aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
        aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
        aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
        // TOP
        aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
        aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
        aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
        aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
        // BOTTOM
        aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
        aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
        aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
        aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
    };
    m_textureCoordsBuffer->update(&textureCoords, 24 * 2 * sizeof(GLfloat));
}