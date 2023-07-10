#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

#include <string>

namespace RenderEngine
{
	class Renderer
	{
	public:
		static void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader);
		static void bindTexture(const Texture2D& texture);
		static void setClearColor(const float r, const float g, const float b, const float a = 1.0f);
		static void clearColor();
		static void setViewport(unsigned int width, unsigned int height, unsigned int offsetLeft = 0, unsigned int offsetBottom = 0);

		static std::string getRendererStr();
		static std::string getVersionStr();
	};
}