#pragma once

typedef unsigned int GLuint;

//#include <glad/glad.h>
namespace RenderEngine
{
	class FrameBuffer
	{
	public:
		FrameBuffer();
		~FrameBuffer();

		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer(FrameBuffer&& FrameBuffer) noexcept;
		FrameBuffer& operator=(const FrameBuffer&) = delete;
		FrameBuffer& operator=(FrameBuffer&& FrameBuffer) noexcept;

		bool init(int window_width, int window_height);
		void bind();
		void unbind();

		void bind_texture();
		void bind_texture_depth();
		void bind_texture_no_effects();
		void unbind_textures();

		unsigned char* get_screenshot(int window_width, int window_height);

		void on_resize(int window_width, int window_height);

		GLuint getID() { return m_ID; }
	private:
		bool m_is_init;

		GLuint m_ID;
		GLuint m_ID_tex;
		GLuint m_ID_tex_depth;
		GLuint m_ID_tex_neff;
		GLuint m_ID_rbo;
	};
	class FrameBufferMultuSampled
	{
	public:
		FrameBufferMultuSampled();
		~FrameBufferMultuSampled();

		FrameBufferMultuSampled(const FrameBufferMultuSampled&) = delete;
		FrameBufferMultuSampled(FrameBufferMultuSampled&& FrameBuffer) noexcept;
		FrameBufferMultuSampled& operator=(const FrameBufferMultuSampled&) = delete;
		FrameBufferMultuSampled& operator=(FrameBufferMultuSampled&& FrameBuffer) noexcept;

		bool init(int window_width, int window_height, int samples);

		void use(GLuint id);
		void bind();
		void unbind();

		void bind_texture();
		void bind_texture_depth();
		void unbind_textures();

		void on_resize(int window_width, int window_height);

		void set_samples_count(unsigned int samples);
	private:
		unsigned int m_samples;

		unsigned int m_width;
		unsigned int m_height;

		bool m_is_init;

		GLuint m_ID;
		GLuint m_ID_tex;
		GLuint m_ID_tex_depth;
		GLuint m_ID_rbo;
	};
}