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
	std::vector<std::string> Material::get_shader_prop_names(ETypeData type)
	{
		std::vector<std::string> names;

		auto elements = m_pShaderProgram->get_layout()->getLayoutElements();

		for (const auto& curElement : elements)
		{
			if (curElement.type == type) names.push_back(curElement.name);
		}

		return names;
	}

	void Material::set_model_matrix(glm::mat4& model)
	{
		m_pShaderProgram->setMatrix4(get_shader_prop_names(ModelMat4)[0], model);
	}
	void Material::set_view_projection_matrix(glm::mat4& vp)
	{
		m_pShaderProgram->setMatrix4(get_shader_prop_names(VPMat4)[0], vp);
	}
	void Material::set_first_vec4(glm::vec4& value)
	{
		m_pShaderProgram->setVec4(get_shader_prop_names(Vec4)[0], value);
	}
	std::shared_ptr<Texture2D> Material::get_texture_ptr()
	{
		return m_pTexture;
	}
	std::shared_ptr<ShaderProgram> Material::get_shader_ptr()
	{
		return m_pShaderProgram;
	}
	void Material::set_shader_and_texture(std::shared_ptr<ShaderProgram> pShaderProgram, std::shared_ptr<Texture2D> pTexture)
	{
		m_pTexture = std::move(pTexture);
		m_pShaderProgram = std::move(pShaderProgram);
	}
}