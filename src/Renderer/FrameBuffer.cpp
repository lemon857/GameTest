#include "EngineCore/Renderer/FrameBuffer.h"
#include "EngineCore/Renderer/Renderer.h"

#include <glad/glad.h>

RenderEngine::FrameBuffer::FrameBuffer()
	: m_ID(0)
	, m_ID_tex(0)
	, m_ID_tex_neff(0)
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
	glDeleteTextures(1, &m_ID_tex_neff);
	glDeleteTextures(1, &m_ID_tex_depth);
	glDeleteRenderbuffers(1, &m_ID_rbo);
}

RenderEngine::FrameBuffer::FrameBuffer(FrameBuffer&& FrameBuffer) noexcept
{
	m_ID = FrameBuffer.m_ID;
	m_ID_tex = FrameBuffer.m_ID_tex;
	m_ID_tex_neff = FrameBuffer.m_ID_tex_neff;
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
	m_ID_tex_neff = FrameBuffer.m_ID_tex_neff;
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

	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
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

unsigned char* RenderEngine::FrameBuffer::get_screenshot(int window_width, int window_height)
{
	unsigned char* buffer = new unsigned char[window_width * window_height * 4];
	glBindTexture(GL_TEXTURE_2D, m_ID_tex);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glBindTexture(GL_TEXTURE_2D, 0);
	return buffer;
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

RenderEngine::FrameBufferMultuSampled::FrameBufferMultuSampled()
	: m_ID(0)
	, m_ID_tex(0)
	, m_ID_tex_depth(0)
	, m_ID_rbo(0)
	, m_is_init(false)
	, m_width(0)
	, m_height(0)
{
}

RenderEngine::FrameBufferMultuSampled::~FrameBufferMultuSampled()
{
	if (!m_is_init) return;
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(1, &m_ID_tex);
	glDeleteRenderbuffers(1, &m_ID_rbo);
}

RenderEngine::FrameBufferMultuSampled::FrameBufferMultuSampled(FrameBufferMultuSampled&& FrameBuffer) noexcept
{
	m_ID = FrameBuffer.m_ID;
	m_ID_tex = FrameBuffer.m_ID_tex;
	m_ID_tex_depth = FrameBuffer.m_ID_tex_depth;
	m_ID_rbo = FrameBuffer.m_ID_rbo;
	m_is_init = FrameBuffer.m_is_init;
	m_width = FrameBuffer.m_width;
	m_height = FrameBuffer.m_height;
	FrameBuffer.m_ID = 0;
	m_ID_tex = 0;
	m_ID_rbo = 0;
	m_is_init = false;
}

RenderEngine::FrameBufferMultuSampled& RenderEngine::FrameBufferMultuSampled::operator=(FrameBufferMultuSampled&& FrameBuffer) noexcept
{
	m_ID = FrameBuffer.m_ID;
	m_ID_tex = FrameBuffer.m_ID_tex;
	m_ID_tex_depth = FrameBuffer.m_ID_tex_depth;
	m_ID_rbo = FrameBuffer.m_ID_rbo;
	m_is_init = FrameBuffer.m_is_init;
	m_width = FrameBuffer.m_width;
	m_height = FrameBuffer.m_height;
	FrameBuffer.m_ID = 0;
	m_ID_tex = 0;
	m_ID_rbo = 0;
	m_is_init = false;
	return *this;
}

bool RenderEngine::FrameBufferMultuSampled::init(int window_width, int window_height, int samples)
{
	m_samples = samples;
	m_width = window_width;
	m_height = window_height;
	glGenFramebuffers(1, &m_ID);

	glGenTextures(1, &m_ID_tex);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ID_tex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_RGB, window_width, window_height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenTextures(1, &m_ID_tex_depth);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ID_tex_depth);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_DEPTH_COMPONENT, window_width, window_height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenRenderbuffers(1, &m_ID_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID_rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH24_STENCIL8, window_width, window_height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ID_tex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_ID_tex_depth, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_ID_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) return false;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	m_is_init = true;

	return true;
}

void RenderEngine::FrameBufferMultuSampled::use(GLuint id)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_ID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
}

void RenderEngine::FrameBufferMultuSampled::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void RenderEngine::FrameBufferMultuSampled::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderEngine::FrameBufferMultuSampled::bind_texture()
{
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ID_tex);
}

void RenderEngine::FrameBufferMultuSampled::bind_texture_depth()
{
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ID_tex_depth);
}

void RenderEngine::FrameBufferMultuSampled::unbind_textures()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void RenderEngine::FrameBufferMultuSampled::on_resize(int window_width, int window_height)
{
	if (!m_is_init) return;
	m_width = window_width;
	m_height = window_height;
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ID_tex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_RGB, window_width, window_height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ID_tex_depth);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_DEPTH_COMPONENT, window_width, window_height, GL_TRUE);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID_rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH24_STENCIL8, window_width, window_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	unbind_textures();
}

void RenderEngine::FrameBufferMultuSampled::set_samples_count(unsigned int samples)
{
	if (!m_is_init) return;
	m_samples = samples;
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ID_tex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_RGB, m_width, m_height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ID_tex_depth);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_DEPTH_COMPONENT, m_width, m_height, GL_TRUE);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ID_rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH24_STENCIL8, m_width, m_height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	unbind_textures();
}
