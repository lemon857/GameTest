#include "EngineCore/Renderer/FrameBuffer.h"
#include "EngineCore/Renderer/Renderer.h"

#include <glad/glad.h>

RenderEngine::FrameBuffer::FrameBuffer()
	: m_ID(0)
{

}

RenderEngine::FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_ID);
}

RenderEngine::FrameBuffer::FrameBuffer(FrameBuffer&& FrameBuffer) noexcept
{
	m_ID = FrameBuffer.m_ID;
	FrameBuffer.m_ID = 0;
}

RenderEngine::FrameBuffer& RenderEngine::FrameBuffer::operator=(FrameBuffer&& FrameBuffer) noexcept
{
	m_ID = FrameBuffer.m_ID;
	FrameBuffer.m_ID = 0;
	return *this;
}

bool RenderEngine::FrameBuffer::init(int window_width, int window_height)
{
	glGenFramebuffers(1, &m_ID);

	glGenTextures(1, &m_ID_tex);
	glBindTexture(GL_TEXTURE_2D, m_ID_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenRenderbuffers(1, &m_ID_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ID_tex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_ID_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) return false;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void RenderEngine::FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void RenderEngine::FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void RenderEngine::FrameBuffer::bind_texture()
{
	glBindTexture(GL_TEXTURE_2D, m_ID_tex);
}
void RenderEngine::FrameBuffer::unbind_texture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

