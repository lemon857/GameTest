#pragma once

#include <memory>
#include <string>

namespace RenderEngine
{
	class ShaderProgram;
	class Texture2D;

	class Material
	{
	public:
		Material(std::shared_ptr<ShaderProgram> pShaderProgram, std::shared_ptr<Texture2D> pTexture);

		void use();

	private:
		std::shared_ptr<ShaderProgram> m_pShaderProgram;
		std::shared_ptr<Texture2D> m_pTexture;
	};
}