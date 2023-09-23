#include "EngineCore/GUI/GUI_place.h"
#include "EngineCore/GUI/GUI_element.h"

#include "EngineCore/Renderer/Material.h"
#include "EngineCore/Renderer/ShaderProgram.h"

#include "EngineCore/System/ShadersSettings.h"
#include "EngineCore/Renderer/Camera.h"

namespace GUI
{
	GUI_place::GUI_place(Camera* render_cam, std::shared_ptr<RenderEngine::Material> pMaterial)
		: m_pMaterial(std::move(pMaterial))
		, m_render_cam(std::move(render_cam))
	{
	}

	void GUI_place::on_update(const double delta)
	{
		for (auto cur : m_elements)
		{
			cur->on_update(delta);
		}
	}
	void GUI_place::on_render()
	{
		m_pMaterial->use();
		m_pMaterial->get_shader_ptr()->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, m_render_cam->get_ui_matrix());
		for (auto cur : m_elements)
		{
			cur->on_render();
		}
	}
	void GUI_place::add_element(GUI_element* element)
	{
		m_elements.push_back(element);
	}
}