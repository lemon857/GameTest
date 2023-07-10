#include "IndexBuffer.h"


namespace Renderer
{
	IndexBuffer::IndexBuffer()
		: m_ID(0)
	{

	}
	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}
	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& IndexBuffer) noexcept
	{
		m_ID = IndexBuffer.m_ID;
		IndexBuffer.m_ID = 0;
		return *this;
	}
	IndexBuffer::IndexBuffer(IndexBuffer&& IndexBuffer) noexcept
	{
		m_ID = IndexBuffer.m_ID;
		IndexBuffer.m_ID = 0;
	}
	void IndexBuffer::init(const void* data, const unsigned int size)
	{
		glGenBuffers(1, &m_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	}
	void IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}