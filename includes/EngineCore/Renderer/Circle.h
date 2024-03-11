#pragma once

#include <memory>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


namespace RenderEngine
{
	class Material;
	class VertexArray;
	class VertexBuffer;

	class Circle
	{
	public:
		Circle(Material* pMaterial, glm::vec3 pos, glm::vec3 scale, glm::vec3 rot, glm::vec4 color, float radius, int numSegments, float size = 1.f);
		~Circle();

		void render();

		void update(float radius, int numSegments);

		void set_pos(glm::vec3 pos);
		void set_rot(glm::vec3 rot);
		void set_rad(float radius);

		void add_rot(glm::vec3 rot);

	private:
		Material* m_pMaterial;
		float m_size;
		float m_radius;
		int m_numSegments;

		glm::vec3 m_pos;
		glm::vec3 m_scale;
		glm::vec3 m_rot;

		glm::vec4 m_color;

		VertexArray* m_vertexArray;
		VertexBuffer* m_vertexCoordsBuffer;
	};
}