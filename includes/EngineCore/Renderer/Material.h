#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/mat4x4.hpp>

#include "EngineCore/Renderer/ShaderProgramLayout.h"

namespace RenderEngine
{
	class ShaderProgram;
	class Texture2D;

	class Material
	{
	public:
		Material(std::shared_ptr<ShaderProgram> pShaderProgram, std::shared_ptr<Texture2D> pTexture);

		void use();

		std::vector<std::string> get_shader_prop_names(ETypeData type);

		void set_model_matrix(glm::mat4& model);
		void set_view_projection_matrix(glm::mat4& vp);

		std::shared_ptr<Texture2D> get_texture_ptr();

	private:
		std::shared_ptr<ShaderProgram> m_pShaderProgram;
		std::shared_ptr<Texture2D> m_pTexture;
	};
}