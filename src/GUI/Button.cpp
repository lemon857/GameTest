#include "EngineCore/GUI/Button.h"

#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/GUI/FontRenderer.h"
#include "EngineCore/GUI/Font.h"
#include "EngineCore/GUI/Sprite.h"

#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Resources/ResourceManager.h"

namespace GUI
{
	Button::Button(Sprite* face, std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec2 pos, glm::vec2 scale,
		std::string text, std::string shaderName, std::shared_ptr<Font> font, glm::vec3 textColor)
		: GUI_element(std::move(pMaterial), "button")
		, m_textRenderer(std::make_unique<FontRenderer>(font, ResourceManager::getShaderProgram(shaderName)))
		, m_face(std::move(face))
		, m_color(textColor)
	{
		m_position = pos;
		m_scale = scale;
		m_face->set_position(pos);
		m_face->set_scale(scale);
	}
	Button::~Button()
	{
		
	}
	void Button::on_render_prj(glm::mat4& prj)
	{
		//m_face->on_render_prj(prj);
		m_textRenderer->render_text(m_text, m_position.x, m_position.y, 1.f, m_color, prj);
	}
}