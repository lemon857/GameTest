#pragma once

#include "EngineCore/GUI/GUI_element.h"

#include <glm/mat4x4.hpp>

namespace RenderEngine
{
	class Material;
	class Texture2D;
	class ShaderProgram;
	class VertexBuffer;
	class VertexArray;
	class IndexBuffer;
}

namespace GUI
{
	class Sprite : public GUI_element
	{
	public:
		Sprite(RenderEngine::Material* pMaterial, std::string initSubTexture = "default",
			glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(0), std::string name = "default");

		Sprite(RenderEngine::ShaderProgram* pShader, RenderEngine::Texture2D* pTexture, std::string initSubTexture = "default",
			glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(0), std::string name = "default");

		~Sprite();
		
		void setSubTexture(std::string subTexture);

		void on_render_prj(glm::mat4 prj) override;
	private:
		RenderEngine::VertexArray* m_vertexArray;
		RenderEngine::VertexBuffer* m_vertexCoordsBuffer;
		RenderEngine::VertexBuffer* m_textureCoordsBuffer;
		RenderEngine::IndexBuffer* m_indexBuffer;
	};
}
