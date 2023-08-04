#include "Renderer.h"

namespace RenderEngine
{
	void Renderer::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader)
	{
		shader.use();
		vertexArray.bind();
		indexBuffer.bind();
		glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, 0);
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
}