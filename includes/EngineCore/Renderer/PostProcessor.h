#pragma once

#include <memory>

typedef unsigned int GLuint;

namespace RenderEngine
{
	class FrameBuffer;
	class FrameBufferMultuSampled;
	class ShaderProgram;

	class PostProcessor
	{
	public:
		PostProcessor(std::shared_ptr<ShaderProgram> screen_shader, int window_width, int window_height, bool active_msaa = false, unsigned int samples = 4);
		~PostProcessor();

		// called before rendering main scene
		void start_render();
		// called after rendering main scene
		void end_render();

		void set_active(bool state);
		void change_active();

		void set_acive_msaa(bool state);
		void change_acive_msaa();

		void on_resize(int window_width, int window_height);

		void set_effect(int num);
		void set_samples_count(unsigned int samples);

		bool get_active() { return m_isActive; }

		unsigned char* get_screenshot(int window_width, int window_height);

		// for reading pixels in fbo
		void bind();
		void unbind();
	private:
		int m_cur_effect;

		bool m_isActive;
		bool m_activeMSAA;

		GLuint m_quadVAO;
		GLuint m_quadVBO;

		FrameBuffer* m_fbo;
		FrameBufferMultuSampled* m_fbo_ms;

		std::shared_ptr<ShaderProgram> m_screen_shader;
	};
}