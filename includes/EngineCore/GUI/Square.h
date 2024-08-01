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
		Square(RenderEngine::Material* pMaterial, glm::vec2 pos, glm::vec2 scale);
		~Square();

		void on_render_prj(glm::mat4 prj) override;

		void set_color(glm::vec3 color);
	private:
		glm::vec3 m_color;

		RenderEngine::VertexArray* m_vertexArray;
		RenderEngine::VertexBuffer* m_vertexCoordsBuffer;
		RenderEngine::IndexBuffer* m_indexBuffer;
	};
}
