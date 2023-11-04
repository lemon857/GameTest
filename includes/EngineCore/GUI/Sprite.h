#pragma once

#include "EngineCore/GUI/GUI_element.h"

#include <glm/mat4x4.hpp>

namespace RenderEngine
{
	class Material;
	class VertexBuffer;
	class VertexArray;
	class IndexBuffer;
}

namespace GUI
{
	class Sprite : public GUI_element
	{
	public:
		Sprite(std::shared_ptr<RenderEngine::Material> pMaterial, std::string initSubTexture = "default", glm::vec2 pos = glm::vec2(0), glm::vec2 scale = glm::vec2(0));

		~Sprite();
		
		void setSubTexture(std::string subTexture);

		void on_render_prj(glm::mat4& prj) override;
	private:
		std::shared_ptr<RenderEngine::VertexArray> m_vertexArray;
		RenderEngine::VertexBuffer* m_vertexCoordsBuffer;
		RenderEngine::VertexBuffer* m_textureCoordsBuffer;
		RenderEngine::IndexBuffer* m_indexBuffer;
	};
}