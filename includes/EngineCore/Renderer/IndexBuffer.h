#pragma once

typedef unsigned int GLuint;

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

		bool init(const void* data, const unsigned int count);
		void bind() const;
		void unbind() const;
		unsigned int getCount() const { return m_count; };
	private:
		GLuint m_ID;
		unsigned int m_count;
	};
}