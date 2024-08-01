#pragma once

#include <string>
#include <vector>
#include <glm/mat4x4.hpp>

namespace RenderEngine
{
	class Texture3D;
	class ShaderProgram;

	class Skybox
	{
	public:
		Skybox(std::vector<std::string> faces, ShaderProgram* shader);
		Skybox(Texture3D* texture, ShaderProgram* shader);
		~Skybox();

		void render(glm::mat4 prj);

	private:
		unsigned int m_cube;
		Texture3D* m_texture;
		ShaderProgram* m_shader;
	};
}
