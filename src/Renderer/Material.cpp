#include "EngineCore/Renderer/Material.h"

#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/Texture2D.h"
#include "EngineCore/Renderer/Renderer.h"

namespace RenderEngine
{
	Material::Material(std::shared_ptr<ShaderProgram> pShaderProgram, std::shared_ptr<Texture2D> pTexture)
		: m_pShaderProgram(std::move(pShaderProgram))
		, m_pTexture(std::move(pTexture))
	{

	}

	void Material::use()
	{
		m_pShaderProgram->use();
		if (m_pTexture != nullptr) Renderer::bindTexture(*m_pTexture);
	}
}