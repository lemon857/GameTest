#include "EngineCore/Renderer/PostProcessor.h"
#include "EngineCore/Renderer/FrameBuffer.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/System/ShadersSettings.h"

RenderEngine::PostProcessor::PostProcessor(std::shared_ptr<ShaderProgram> screen_shader, int window_width, int window_height, bool active_msaa, unsigned int samples)
	: m_screen_shader(std::move(screen_shader))
	, m_isActive(false)
	, m_activeMSAA(active_msaa)
	, m_quadVAO(0)
	, m_quadVBO(0)
	, m_cur_effect(0)
	, m_fbo(new FrameBuffer())
	, m_fbo_ms(new FrameBufferMultuSampled())
{
	m_fbo->init(window_width, window_height);
	m_fbo_ms->init(window_width, window_height, samples);
	float quadVert[] =
	{
		-1.f, 1.f, 0.f, 1.f,
		-1.f, -1.f, 0.f, 0.f,
		1.f, -1.f, 1.f, 0.f,

		-1.f, 1.f, 0.f, 1.f,
		1.f, -1.f, 1.f, 0.f,
		1.f, 1.f, 1.f, 1.f
	};
	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &m_quadVBO);
	glBindVertexArray(m_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVert), &quadVert, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	m_screen_shader->use();
	m_screen_shader->setInt(SS_RENDER_TEXTURE_NAME, 0);
	m_screen_shader->setInt(SS_DEPTH_TEXTURE_NAME, 1);
	//m_screen_shader->setInt("realTexture", 2);
}

RenderEngine::PostProcessor::~PostProcessor()
{
	delete m_fbo;
	delete m_fbo_ms;
}

void RenderEngine::PostProcessor::start_render()
{
	if (!m_isActive) return;
	RenderEngine::Renderer::setDepthTest(true);
	if (m_activeMSAA) m_fbo_ms->bind();
	else m_fbo->bind();
}

void RenderEngine::PostProcessor::end_render()
{
	if (!m_isActive) return; 
	glBindVertexArray(0);

	if (m_activeMSAA)
	{
		m_fbo->bind();
		RenderEngine::Renderer::clear(false);
		m_fbo_ms->use(m_fbo->getID());
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	RenderEngine::Renderer::setDepthTest(false);
	RenderEngine::Renderer::setClearColor(1.f, 1.f, 1.f, 1.f);
	RenderEngine::Renderer::clear(true);

	glActiveTexture(GL_TEXTURE0);
	m_fbo->bind_texture();
	glActiveTexture(GL_TEXTURE1);
	m_fbo->bind_texture_depth();
	glBindVertexArray(m_quadVAO);

	m_screen_shader->use();
	m_screen_shader->setInt("cur_effect", m_cur_effect);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	m_fbo->unbind_textures();

	RenderEngine::Renderer::clear(false);
	RenderEngine::Renderer::setDepthTest(true);
}

void RenderEngine::PostProcessor::set_active(bool state)
{
	m_isActive = state;
	m_fbo_ms->unbind();
	m_fbo->unbind();
}

void RenderEngine::PostProcessor::change_active()
{
	m_isActive = !m_isActive;
	m_fbo_ms->unbind();
	m_fbo->unbind();
}

void RenderEngine::PostProcessor::set_acive_msaa(bool state)
{
	m_activeMSAA = state;
}
void RenderEngine::PostProcessor::change_acive_msaa()
{
	m_activeMSAA = !m_activeMSAA;
}

void RenderEngine::PostProcessor::on_resize(int window_width, int window_height)
{
	m_fbo->on_resize(window_width, window_height);
	m_fbo_ms->on_resize(window_width, window_height);
}

void RenderEngine::PostProcessor::set_effect(int num)
{
	m_cur_effect = num;
}

void RenderEngine::PostProcessor::set_samples_count(unsigned int samples)
{
	m_fbo_ms->set_samples_count(samples);
}
void RenderEngine::PostProcessor::bind()
{
	m_fbo->bind();
}
void RenderEngine::PostProcessor::unbind()
{
	m_fbo->unbind();
}
unsigned char* RenderEngine::PostProcessor::get_screenshot(int window_width, int window_height)
{
	return m_fbo->get_screenshot(window_width, window_height);
}