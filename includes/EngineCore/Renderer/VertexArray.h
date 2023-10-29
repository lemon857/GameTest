#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

namespace RenderEngine
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray(VertexArray&& VertexArray) noexcept;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&& VertexArray) noexcept;

		void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
		void bind() const;
		void unbind() const;

		unsigned int count() const;
	private:
		GLuint m_ID = 0;
		unsigned int m_elementsCount = 0;
	};
}