#pragma once

#include <memory>

typedef unsigned int GLuint;

namespace RenderEngine
{
	class FrameBuffer;
	class ShaderProgram;

	class PostProcessor
	{
	public:
		PostProcessor(std::shared_ptr<ShaderProgram> screen_shader, int window_width, int window_height);
		~PostProcessor();

		// called before rendering main scene
		void start_render();
		// called after rendering main scene
		void end_render();
		// called before rendering gui
		void start_gui_rendering();

		void set_active(bool state);
		void change_active();

		void on_resize(int window_width, int window_height);
	private:
		bool m_isActive;

		GLuint m_quadVAO;
		GLuint m_quadVBO;

		FrameBuffer* m_fbo;

		std::shared_ptr<ShaderProgram> m_screen_shader;
	};
}