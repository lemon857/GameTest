#pragma once

#include <string>
#include <vector>

typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace RenderEngine
{
	class Texture3D
	{
	public:
		// need 6 textures in data
		// right
		// left
		// top
		// bottom
		// front
		// back
		Texture3D(std::vector<std::string> faces_rel_path);
		~Texture3D();

		Texture3D() = delete;
		Texture3D(const Texture3D&) = delete;
		Texture3D& operator=(const Texture3D&) = delete;
		Texture3D& operator=(Texture3D&& texture2D) noexcept;
		Texture3D(Texture3D&& texture2D) noexcept;

		void bind() const;

	private:
		GLuint m_ID;
	};
}