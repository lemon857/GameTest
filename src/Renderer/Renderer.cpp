#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/System/Log.h"

#include <glad/glad.h>

namespace RenderEngine
{
	bool Renderer::init(GLFWwindow* pWindow)
	{
		if (!glfwInit())
		{
			LOG_CRIT("GLFW init failed");
			return false;
		}
		return true;
	}
	void Renderer::drawTriangles(const VertexArray& vertexArray, const IndexBuffer& indexBuffer)
	{
		vertexArray.bind();
		indexBuffer.bind();
		glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, 0);
	}
	void Renderer::drawPoint(const VertexArray& vertexArray, const ShaderProgram& shader, const GLfloat size)
	{
		shader.use();
		vertexArray.bind();
		glPointSize(size);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	void Renderer::drawLine(const VertexArray& vertexArray, const GLfloat size)
	{
		vertexArray.bind();
		glLineWidth(size);
		glDrawArrays(GL_LINES, 0, 2);
	}
	void Renderer::bindTexture(const Texture2D& texture)
	{
		glActiveTexture(GL_TEXTURE0);
		texture.bind();
	}
	void Renderer::setClearColor(const float r, const float g, const float b, const float a)
	{
		glClearColor(r, g, b, a);
	}
	void Renderer::setDepthTest(const bool enable)
	{
		if (enable) 
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}
	void Renderer::clearColor() 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void Renderer::setViewport(unsigned int width, unsigned int height, unsigned int offsetLeft, unsigned int offsetBottom)
	{
		glViewport(offsetLeft, offsetBottom, width, height);
	}
	std::string Renderer::getRendererStr()
	{
		return (char*)glGetString(GL_RENDERER);
	}
	std::string Renderer::getVersionStr()
	{
		return (char*)glGetString(GL_VERSION);
	}
	float Renderer::get_depth_pixel(float pix_x, float pix_y, float window_height)
	{
		GLfloat depth = 0.f;
		glReadPixels(pix_x, window_height - pix_y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		return depth;
	}
	int Renderer::get_info_pixel(float pix_x, float pix_y, float window_height)
	{
		glFlush();
		glFinish();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		unsigned char data[4];
		glReadPixels(pix_x, window_height - pix_y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data); 
		int pickedID =
			data[0] +
			data[1] * 256 +
			data[2] * 256 * 256;
		return pickedID;
	}
}