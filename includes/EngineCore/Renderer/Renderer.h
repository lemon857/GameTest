#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

#include <string>

struct GLFWwindow;

namespace RenderEngine
{
	struct PixelInfo {
		GLuint ObjectID = 0;
		GLuint DrawID = 0;
		GLuint PrimID = 0;
	};

	class Renderer
	{
	public:
		static bool init();

		static void drawTriangles(const VertexArray& vertexArray, const IndexBuffer& indexBuffer);

		static void drawTriangles(const VertexArray& vertexArray);

		static void drawPoint(const VertexArray& vertexArray, const ShaderProgram& shader, const GLfloat size);

		static void drawLine(const VertexArray& vertexArray, const GLfloat size, const int countPoints = 2);

		static void bindTexture(const Texture2D& texture);

		static void setClearColor(const float r, const float g, const float b, const float a = 1.0f);
		static void setDepthTest(const bool enable);
		static void setBlend (const bool enable);
		static void setStencilTest(const bool enable);
		static void setStencilMask(const bool enable);
		static void clear();
		static void clear(const bool iscolor);
		static void setViewport(unsigned int width, unsigned int height, unsigned int offsetLeft = 0, unsigned int offsetBottom = 0);

		static std::string getRendererStr();
		static std::string getVersionStr();

		static float get_depth_pixel(float pix_x, float pix_y, float window_height);
		static int get_info_pixel(float pix_x, float pix_y, float window_height);
	private:
		static bool s_enable_depth;
		static bool s_enable_blend;

		static float s_color_r;
		static float s_color_g;
		static float s_color_b;
		static float s_color_a;
	};
}