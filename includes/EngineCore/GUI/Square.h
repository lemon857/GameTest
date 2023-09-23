#pragma once

#include "EngineCore/GUI/GUI_element.h"

#include <glm/vec3.hpp>

namespace RenderEngine
{
	class Material;
	class VertexBuffer;
	class VertexArray;
	class IndexBuffer;
}

namespace GUI
{
	class Square : public GUI_element
	{
	public:
		Square(std::shared_ptr<RenderEngine::Material> pMaterial, glm::ivec2 pos, glm::ivec2 scale);
		~Square();

		void on_render() override;

		void set_color(glm::vec3 color);
	private:
		glm::vec3 m_color;

		std::shared_ptr<RenderEngine::VertexArray> m_vertexArray;
		RenderEngine::VertexBuffer* m_vertexCoordsBuffer;
		RenderEngine::IndexBuffer* m_indexBuffer;
	};
}