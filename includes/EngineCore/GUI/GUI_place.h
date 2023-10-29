#pragma once

#include <vector>
#include <memory>

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
		~GUI_place() = default;

		void on_update(const double delta);

		void on_render();

		void add_element(GUI_element* element);

		GUI_element* get_element(size_t index);

		void on_mouse_click(int x, int y);
	private:
		Camera* m_render_cam;
		std::shared_ptr<RenderEngine::Material> m_pMaterial;
		std::vector<GUI_element*> m_elements;
	};
}