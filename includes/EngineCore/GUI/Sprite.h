#pragma once

#include "EngineCore/GUI/GUI_element.h"

namespace RenderEngine
{
	class Material;
	class VertexBuffer;
	class VertexArray;
	class IndexBuffer;
	class ShaderProgram;

}

namespace GUI
{
	class Sprite : public GUI_element
	{
	public:
		Sprite(std::shared_ptr<RenderEngine::Material> pMaterial, std::string initSubTexture = "default");

		~Sprite();
		
		void setSubTexture(std::string subTexture);

		void on_update(const double delta) override;
	private:
		std::shared_ptr<RenderEngine::VertexArray> m_vertexArray;
		RenderEngine::VertexBuffer* m_vertexCoordsBuffer;
		RenderEngine::VertexBuffer* m_textureCoordsBuffer;
		RenderEngine::IndexBuffer* m_indexBuffer;
	};
}