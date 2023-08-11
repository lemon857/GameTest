#pragma once

#include <memory>
#include <string>
#include <glm/vec3.hpp>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace RenderEngine 
{
	class Texture2D;
	class ShaderProgram;

	class Sprite 
	{
	public:
		Sprite(std::shared_ptr<Texture2D> pTexture,
			std::string initialSubTexture,
			std::shared_ptr<ShaderProgram> pShaderProgram);

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		void setSubTexture(std::string subTexture);

		void render(const glm::vec3& position, const glm::vec3& size, const double rotation) const;
	private:
		std::shared_ptr<Texture2D> m_pTextureAtlas;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;

		VertexArray m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_textureCoordsBuffer;
		IndexBuffer m_indexBuffer; 
	};
}