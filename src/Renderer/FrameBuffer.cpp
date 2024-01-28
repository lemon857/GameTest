#include "EngineCore/Renderer/FrameBuffer.h"
#include "EngineCore/Renderer/Renderer.h"

#include <glad/glad.h>

RenderEngine::FrameBuffer::FrameBuffer()
	: m_ID(0)
	, m_ID_tex(0)
	, m_ID_tex_depth(0)
	, m_ID_rbo(0)
	, m_is_init(false)
{

}

RenderEngine::FrameBuffer::~FrameBuffer()
{
	if (!m_is_init) return;
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(1, &m_ID_tex);
	glDeleteTextures(1, &m_ID_tex_depth);
	glDeleteRenderbuffers(1, &m_ID_rbo);
}

RenderEngine::FrameBuffer::FrameBuffer(FrameBuffer&& FrameBuffer) noexcept
{
	m_ID = FrameBuffer.m_ID;
	m_ID_tex = FrameBuffer.m_ID_tex;
	m_ID_tex_depth = FrameBuffer.m_ID_tex_depth;
	m_ID_rbo = FrameBuffer.m_ID_rbo;
	m_is_init = FrameBuffer.m_is_init;
	FrameBuffer.m_ID = 0;
	m_ID_tex = 0;
	m_ID_tex_depth = 0;
	m_ID_rbo = 0;
	m_is_init = false;
}

RenderEngine::FrameBuffer& RenderEngine::FrameBuffer::operator=(FrameBuffer&& FrameBuffer) noexcept
{
	m_ID = FrameBuffer.m_ID;
	m_ID_tex = FrameBuffer.m_ID_tex;
	m_ID_tex_depth = FrameBuffer.m_ID_tex_depth;
	m_ID_rbo = FrameBuffer.m_ID_rbo;
	m_is_init = FrameBuffer.m_is_init;
	FrameBuffer.m_ID = 0;
	m_ID_tex = 0;
	m_ID_tex_depth = 0;
	m_ID_rbo = 0;
	m_is_init = false;
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

	glGenTextures(1, &m_ID_tex_depth);
	glBindTexture(GL_TEXTURE_2D, m_ID_tex_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &m_ID_tex_neff);
	glBindTexture(GL_TEXTURE_2D, m_ID_tex_neff);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenRenderbuffers(1, &m_ID_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_ID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ID_tex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_ID_tex_neff, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ID_tex_depth, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_ID_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) return false;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	m_is_init = true;

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
void RenderEngine::FrameBuffer::bind_texture_depth()
{
	glBindTexture(GL_TEXTURE_2D, m_ID_tex_depth);
}
void RenderEngine::FrameBuffer::bind_texture_no_effects()
{
	glBindTexture(GL_TEXTURE_2D, m_ID_tex_neff);
}
void RenderEngine::FrameBuffer::unbind_textures()
{
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderEngine::FrameBuffer::on_resize(int window_width, int window_height)
{
	if (!m_is_init) return;
	glBindTexture(GL_TEXTURE_2D, m_ID_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, m_ID_tex_neff);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, m_ID_tex_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_DEPTH_COMPONENT, NULL);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	unbind_textures();
}

