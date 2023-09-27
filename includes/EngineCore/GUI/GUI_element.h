#pragma once

#include <string>
#include <memory>
#include <functional>

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
			, m_position(glm::vec2(0))
			, m_scale(glm::vec2(0))
			, m_isActive(true)
		{
		}

		~GUI_element() = default;

		virtual void on_update(const double delta) {}
		virtual void on_render() {}

		void set_active(const bool state) { m_isActive = state; }

		void add_position(glm::vec2 pos) { m_position += pos; }
		void set_position(glm::vec2 pos) { m_position = pos; }
		void set_scale(glm::vec2 scale) { m_scale = scale; }

		glm::vec2 get_position() { return m_position; }
		glm::vec2 get_scale() { return m_scale; }

		std::string get_name() { return m_name; }
		bool get_active() { return m_isActive; }

		void on_click() { if (m_on_click != nullptr) m_on_click(); }
		void set_click_callback(std::function<void()> on_click) { m_on_click = on_click; }
	protected:
		bool m_isActive;

		std::function<void()> m_on_click;

		glm::vec2 m_position;
		glm::vec2 m_scale;

		std::string m_name;
		std::shared_ptr<RenderEngine::Material> m_pMaterial;
	};
}