#pragma once

#include <glad/glad.h>
#include <vector>

namespace RenderEngine
{
	struct VertexBufferLayoutElement
	{
		GLuint count;
		GLenum type;
		GLboolean normalized;
		unsigned int size;
	};
	class VertexBufferLayout
	{
	public:
		VertexBufferLayout();

		void reserveElements(const size_t count);
		void addElementLayoutFloat(const unsigned int count, const bool normalized);

		unsigned int getStride() const { return m_stride; };
		const std::vector<VertexBufferLayoutElement>& getLayoutElements() const { return m_layoutElements; };
	private:
		std::vector<VertexBufferLayoutElement> m_layoutElements;
		unsigned int m_stride;
	};
}