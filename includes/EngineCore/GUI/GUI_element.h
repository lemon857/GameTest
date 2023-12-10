#pragma once

#include <string>
#include <memory>
#include <functional>

#include <glm/mat4x4.hpp>

// when you're set position, this is set center text on X coord
#define SHIFT_TEXT_SYMBOL_R_X 20.5f

#define SHIFT_TEXT_SYMBOL_X 10.0f
#define SHIFT_TEXT_SYMBOL_Y 1.2f

#define SHIFT_TEXT_SYMBOL_X_P 1.0f
#define SHIFT_TEXT_SYMBOL_Y_P 1.0f

#define NAME_TEXTURE_STATIC "static"
#define NAME_TEXTURE_CLICKED "clicked"

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
		GUI_element(std::string name, std::shared_ptr<RenderEngine::Material> pMaterial = nullptr)
			: m_name(name)
			, m_pMaterial(std::move(pMaterial))
			, m_position_p(glm::vec2(0))
			, m_scale_p(glm::vec2(0))
			, m_position(glm::vec2(0))
			, m_scale(glm::vec2(0))
			, m_isActive(true)
			, m_isFocused(false)
			, m_layer(0)
		{
		}

		~GUI_element() = default;

		virtual void on_update(const double delta) {}
		virtual void on_render_prj(glm::mat4& prj) {}

		virtual void on_press() {};
		virtual void on_release() {};

		virtual void set_active(const bool state) { m_isActive = state; }

		void add_position(glm::vec2 pos) { m_position += pos; }

		virtual void set_position(glm::vec2 pos) { m_position = pos; }
		virtual void set_scale(glm::vec2 scale) { m_scale = scale; }

		void set_position_p(glm::vec2 pos) { m_position_p = pos; }
		void set_scale_p(glm::vec2 scale) { m_scale_p = scale; }

		glm::vec2 get_position() { return m_position; }
		glm::vec2 get_scale() { return m_scale; }

		glm::vec2 get_position_p() { return m_position_p; }
		glm::vec2 get_scale_p() { return m_scale_p; }

		std::string get_name() { return m_name; }

		bool get_active() { return m_isActive; }
		bool get_focus() { return m_isFocused; }

		void on_click() { if (m_on_click != nullptr) m_on_click(); }
		void set_click_callback(std::function<void()> on_click) { m_on_click = on_click; }

		std::shared_ptr < RenderEngine::Material> get_material() { return m_pMaterial; }

		virtual std::vector<GUI_element*> get_elements() { return std::vector<GUI_element*>(); }
	protected:

		bool m_isFocused;
		bool m_isActive;

		unsigned int m_layer;

		std::function<void()> m_on_click;
		// Real coords
		glm::vec2 m_position;
		glm::vec2 m_scale;
		// % screen range 0 100
		glm::vec2 m_position_p;
		glm::vec2 m_scale_p;

		std::string m_name;
		std::shared_ptr<RenderEngine::Material> m_pMaterial;
	};
}