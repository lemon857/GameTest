#include "EngineCore/Components/SpriteRenderer.h"

#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/IndexBuffer.h"

#include <glm/vec3.hpp>

SpriteRenderer::SpriteRenderer()
	: IComponent()
{

}

SpriteRenderer::~SpriteRenderer()
{
	delete m_indexBuffer;
	delete m_textureCoordsBuffer;
	delete m_vertexCoordsBuffer;
	delete m_vertexArray.get();
}

void SpriteRenderer::init(std::shared_ptr<RenderEngine::Texture2D> pTexture,
	std::string initialSubTexture,
	std::shared_ptr<RenderEngine::ShaderProgram> pShaderProgram)
{

	m_pShaderProgram = std::move(pShaderProgram);
	m_pTextureAtlas = std::move(pTexture);

	m_vertexArray = std::make_shared<RenderEngine::VertexArray>();
	m_vertexCoordsBuffer = new RenderEngine::VertexBuffer();
	m_textureCoordsBuffer = new RenderEngine::VertexBuffer();
	m_indexBuffer = new RenderEngine::IndexBuffer();

	const GLfloat vertexCoords[] = {
		//2--3  1
		//| / /	|
		//1	 3--2	
		//1--2
		//| /|
		//0--3
		//X --- Y
		-0.5f,-0.5f, 0.f,
		-0.5f, 0.5f, 0.f,
		 0.5f, 0.5f, 0.f,
		 0.5f, -0.5f, 0.f
	};

	auto aSubTexture = m_pTextureAtlas->getSubTexture(initialSubTexture);
	const GLfloat textureCoords[] = {
		//U --- V
		aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
		aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
		aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
		aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,
	};
	const GLuint indexes[] = { 0, 1, 2, 2, 3, 0 };

	m_vertexCoordsBuffer->init(&vertexCoords, 3 * 4 * sizeof(GLfloat));
	RenderEngine::VertexBufferLayout vertexCoordsLayout;
	vertexCoordsLayout.addElementLayoutFloat(3, false);
	m_vertexArray->addBuffer(*m_vertexCoordsBuffer, vertexCoordsLayout);

	m_textureCoordsBuffer->init(&textureCoords, 2 * 4 * sizeof(GLfloat));
	RenderEngine::VertexBufferLayout textureCoordsLayout;
	textureCoordsLayout.addElementLayoutFloat(2, false);
	m_vertexArray->addBuffer(*m_textureCoordsBuffer, textureCoordsLayout);

	m_indexBuffer->init(&indexes, 6);

	m_vertexArray->unbind();
	m_indexBuffer->unbind();
}

void SpriteRenderer::setSubTexture(std::string subTexture)
{

}

void SpriteRenderer::update(const double delta)
{

}