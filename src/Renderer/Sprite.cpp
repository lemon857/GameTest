#include "Sprite.h";

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine
{
	Sprite::Sprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram)
		: m_pTextureAtlas(pTexture), m_pShaderProgram(pShaderProgram)
	{
	
		const GLfloat vertexCoords[] = {
			//2--3  1
			//| / /	|
			//1	 3--2	

			//1--2
			//| /|
			//0--3

			//X --- Y
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
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
			
		m_vertexCoordsBuffer.init(&vertexCoords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_textureCoordsBuffer.init(&textureCoords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout textureCoordsLayout;
		textureCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

		m_indexBuffer.init(&indexes, 6);

		m_vertexArray.unbind();
		m_indexBuffer.unbind();
	}
	void Sprite::render(const glm::vec2 position, const glm::vec2 size, const double rotation, const int layer) const
	{
		m_pShaderProgram->use();

		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(position, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, static_cast<float>(glm::radians(rotation)),  glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
		model = glm::scale(model, glm::vec3(size, 1.0f));
		
		m_pShaderProgram->setMatrix4("modelMat", model);
		m_pShaderProgram->setFloat("layer", layer);

		Renderer::bindTexture(*m_pTextureAtlas);

		Renderer::draw(m_vertexArray, m_indexBuffer, *m_pShaderProgram);
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