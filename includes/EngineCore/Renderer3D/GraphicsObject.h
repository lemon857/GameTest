#pragma once

#include <memory>

namespace RenderEngine
{
	class VertexArray;
	class IndexBuffer;
}
// OpenGL buffers
struct GraphicsObject
{
	GraphicsObject()
		: vertex_array(nullptr)
		, index_buffer(nullptr)
	{
	}
	GraphicsObject(GraphicsObject* oldObj)
		: vertex_array(std::move(oldObj->vertex_array))
		, index_buffer(std::move(oldObj->index_buffer))
	{
		delete oldObj;
	}
	GraphicsObject(std::shared_ptr<RenderEngine::VertexArray>& vao,
	std::shared_ptr<RenderEngine::IndexBuffer>& ebo)
		: vertex_array(std::move(vao))
		, index_buffer(std::move(ebo))
	{
	}
	std::shared_ptr<RenderEngine::VertexArray> vertex_array;
	std::shared_ptr<RenderEngine::IndexBuffer> index_buffer;
};