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
		GUI_place(std::shared_ptr<RenderEngine::Material> pMaterial);
		~GUI_place();

		void on_update(const double delta);

	private:
		Camera* m_render_cam;
		std::vector<GUI_element> m_elements;
	};
}