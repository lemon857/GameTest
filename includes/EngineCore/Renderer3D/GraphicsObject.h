#pragma once

#include <memory>

namespace RenderEngine
{
	class VertexArray;
	class IndexBuffer;
}

struct GraphicsObject
{
	GraphicsObject()
		: vertex_array(nullptr)
		, index_buffer(nullptr)
	{
	}

	std::shared_ptr<RenderEngine::VertexArray> vertex_array;
	std::shared_ptr<RenderEngine::IndexBuffer> index_buffer;
};