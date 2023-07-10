#include "AnimatedSprite.h";
#include "Texture2D.h";

#include <iostream>

namespace Renderer
{
	AnimatedSprite::AnimatedSprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram,
		const glm::vec2& position, const glm::vec2& size, const float rotation)
		: Sprite(std::move(pTexture), std::move(initialSubTexture), std::move(pShaderProgram), position, size, rotation)
	{
		m_pCurrentAnimationDuration = m_statesMap.end();
	}

	void AnimatedSprite::insertState(std::string state, std::vector<std::pair<std::string, uint64_t>> subTexturesDuration)
	{
		m_statesMap.emplace(std::move(state), std::move(subTexturesDuration));
	}
	

	void AnimatedSprite::setState(const std::string newState)
	{
		auto it = m_statesMap.find(newState);
		if (it == m_statesMap.end()) 
		{
			std::cout << "Can't find anomation state: " << newState << "\n";
			return;
		}
		if (it != m_pCurrentAnimationDuration) 
		{
			m_currentAnimationTime = 0;
			m_currentFrame = 0;
			m_pCurrentAnimationDuration = it;
			m_ditry = true;
		}
	}

	void AnimatedSprite::update(const uint64_t delta)
	{
		if (m_pCurrentAnimationDuration == m_statesMap.end()) return;

		m_currentAnimationTime += delta;

		while (m_currentAnimationTime >= m_pCurrentAnimationDuration->second[m_currentFrame].second)
		{
			m_currentAnimationTime -= m_pCurrentAnimationDuration->second[m_currentFrame].second;
			++m_currentFrame;
			m_ditry = true;
			if (m_currentFrame == m_pCurrentAnimationDuration->second.size())
			{
				m_currentFrame = 0;
			}
		}
	}

	void AnimatedSprite::render() const
	{
		auto subTexture = m_pTexture->getSubTexture(m_pCurrentAnimationDuration->second[m_currentFrame].first);

		if (m_ditry)
		{
			const GLfloat textureCoords[] = {
				//U --- V
				subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
				subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
				subTexture.rightTopUV.x, subTexture.rightTopUV.y,
				subTexture.rightTopUV.x, subTexture.leftBottomUV.y,

			};

			m_textureCoordsBuffer.update(&textureCoords, 2 * 4 * sizeof(GLfloat));
			//glBindBuffer(GL_ARRAY_BUFFER, m_textureCoords);
			//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(textureCoords), &textureCoords);
			//glBindVertexArray(0);

			m_ditry = false;
		}

		Sprite::render();
	}
}