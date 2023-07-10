#pragma once

#include <glad/glad.h>

namespace RenderEngine
{
	class IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&& IndexBuffer) noexcept;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer& operator=(IndexBuffer&& IndexBuffer) noexcept;

		void init(const void* data, const unsigned int size);

		void bind() const;
		void unbind() const;
	private:
		GLuint m_ID;
	};
}