#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>

#include <glm/vec2.hpp>

class Camera;

namespace RenderEngine
{
	class Material;
}

namespace GUI
{
	class GUI_element;

	class GUI_place
	{
	public:
		GUI_place(Camera* render_cam, std::shared_ptr<RenderEngine::Material> pMaterial);
		~GUI_place() = default; // need add trash clear

		void on_update(const double delta);

		void on_render();

		void add_element(GUI_element* element);

		template <class _Ty, class... _Types>
		_Ty* add_element(_Types&&... _Args)
		{
			GUI_element* element = (GUI_element*)(new _Ty(std::forward<_Types>(_Args)...));
			glm::vec2 posp = element->get_position_p();
			glm::vec2 scalep = element->get_scale_p();
			glm::vec2 wsize = m_render_cam->get_viewport_size();
			element->set_position(glm::vec2(posp.x / 100 * wsize.x, posp.y / 100 * wsize.y));
			element->set_scale(glm::vec2(scalep.x / 100 * wsize.x, scalep.y / 100 * wsize.y));
			m_render_list.push_back(element);
			add_elements(element->get_elements(), wsize);
			m_elements.emplace(element->get_name(), element);
			return (_Ty*)element;
		}		

		template<class _Ty>
		_Ty* get_element(std::string name) 
		{
			std::map<std::string, GUI_element*>::iterator it = m_elements.find(name);
			if (it != m_elements.end())
			{
				return (_Ty*)(it->second);
			}
			return nullptr;
		}

		void on_mouse_press(int x, int y);
		void on_mouse_release(int x, int y);

		void on_resize();

		bool get_focus();
		void set_logging_active(bool active);
		void set_active(bool active);
	private:
		void add_elements(std::vector<GUI_element*> elements, glm::vec2 wsize);		

		Camera* m_render_cam;
		std::shared_ptr<RenderEngine::Material> m_pMaterial;
		
		std::vector<GUI_element*> m_render_list;

		std::map<std::string, GUI_element*> m_elements;
		bool m_isFocus = false;
		bool m_isActive = false;
		bool m_is_event_logging_active = false;
	};
}