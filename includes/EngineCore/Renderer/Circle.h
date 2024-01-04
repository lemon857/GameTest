#pragma once

#include <memory>
#include <glm/vec3.hpp>

namespace RenderEngine
{
	class Material;
	class VertexArray;
	class VertexBuffer;

	class Circle
	{
	public:
		Circle(std::shared_ptr<Material> pMaterial, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot, float radius, int numSegments, float size = 1.f);
		~Circle();

		void render();

		void update(float radius, int numSegments);

	private:
		std::shared_ptr<Material> m_pMaterial;
		float m_size;
		int m_numSegments;

		glm::vec3 m_pos;
		glm::vec3 m_scale;
		glm::vec3 m_rot;

		VertexArray* m_vertexArray;
		VertexBuffer* m_vertexCoordsBuffer;
	};
}