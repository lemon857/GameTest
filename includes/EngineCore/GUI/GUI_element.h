#pragma once

#include <string>
#include <memory>

#include <glm/vec2.hpp>

namespace RenderEngine
{
	class Material;

}

namespace GUI
{
	class GUI_element
	{
	public:
		// require stay material prop first in all childrens
		GUI_element(std::shared_ptr<RenderEngine::Material> pMaterial, std::string name)
			: m_name(name)
			, m_pMaterial(std::move(pMaterial))
			, m_position(glm::ivec2(0))
			, m_scale(glm::ivec2(0))
			, m_isActive(true)
		{
		}

		~GUI_element() = default;

		virtual void on_update(const double delta) {}
		virtual void on_render() {}

		void set_active(const bool state) { m_isActive = state; }

	protected:
		bool m_isActive;

		glm::ivec2 m_position;
		glm::ivec2 m_scale;

		std::string m_name;
		std::shared_ptr<RenderEngine::Material> m_pMaterial;
	};
}