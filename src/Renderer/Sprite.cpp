#include "EngineCore/Renderer/Sprite.h";

#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/Texture2D.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"
#include "EngineCore/Renderer/Renderer.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine
{
	Sprite::Sprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram)
		: m_pTextureAtlas(std::move(pTexture)), m_pShaderProgram(std::move(pShaderProgram))
	{
	
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
			
		m_vertexCoordsBuffer.init(&vertexCoords, 3 * 4 * sizeof(GLfloat));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_textureCoordsBuffer.init(&textureCoords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout textureCoordsLayout;
		textureCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

		m_indexBuffer.init(&indexes, 6);

		m_vertexArray.unbind();
		m_indexBuffer.unbind();
	}
	void Sprite::render(const glm::vec3& position, const glm::vec3& size, const double rotation) const
	{
		m_pShaderProgram->use();

		glm::mat4 model(1.f);

		model = glm::translate(model, position);
		//model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
		//model = glm::rotate(model, static_cast<float>(glm::radians(rotation)),  glm::vec3(0.f, 0.f, 1.f));
		//model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f));
		model = glm::scale(model, size);
		
		m_pShaderProgram->setMatrix4("modelMat", model);

		Renderer::bindTexture(*m_pTextureAtlas);

		Renderer::drawTriangles(m_vertexArray, m_indexBuffer);
	}
	void Sprite::setSubTexture(std::string subTexture)
	{
		auto aSubTexture = m_pTextureAtlas->getSubTexture(subTexture);

		const GLfloat textureCoords[] = {
			//U --- V
			aSubTexture.leftBottomUV.x, aSubTexture.leftBottomUV.y,
			aSubTexture.leftBottomUV.x, aSubTexture.rightTopUV.y,
			aSubTexture.rightTopUV.x, aSubTexture.rightTopUV.y,
			aSubTexture.rightTopUV.x, aSubTexture.leftBottomUV.y,

		};

		m_textureCoordsBuffer.update(&textureCoords, 2 * 4 * sizeof(GLfloat));
	}
}