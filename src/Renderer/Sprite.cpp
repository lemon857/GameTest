#include "Sprite.h";

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "VertexBufferLayout.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RenderEngine
{
	Sprite::Sprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2& position, const glm::vec2& size, const float rotation)
		: m_position(position), m_rotation(rotation), m_size(size), m_pTexture(pTexture), m_pShaderProgram(pShaderProgram)
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

		auto subTexture = m_pTexture->getSubTexture(initialSubTexture);

		const GLfloat textureCoords[] = {				
				//U --- V
				subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
				subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
				subTexture.rightTopUV.x, subTexture.rightTopUV.y,
				subTexture.rightTopUV.x, subTexture.leftBottomUV.y,

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

		m_indexBuffer.init(&indexes, 6 * sizeof(GLuint));

		m_vertexArray.unbind();
		m_indexBuffer.unbind();
	}
	void Sprite::render() const
	{
		m_pShaderProgram->use();

		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(m_position, 0.0f)); 
		model = glm::translate(model, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.0f));
		model = glm::rotate(model, glm::radians(m_rotation),  glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.0f));
		model = glm::scale(model, glm::vec3(m_size, 1.0f));

		m_vertexArray.bind();
		m_pShaderProgram->setMatrix4("modelMat", model);

		glActiveTexture(GL_TEXTURE0);
		m_pTexture->bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		m_vertexArray.unbind();
	}
	void Sprite::setPosition(glm::vec2& position)
	{
		m_position = position;
	}
	void Sprite::setSize(glm::vec2& size)
	{ 
		m_size = size;
	}
	void Sprite::setRotation(const float rotation)
	{
		m_rotation = rotation;
	}
}