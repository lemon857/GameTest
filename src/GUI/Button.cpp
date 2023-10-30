#include "EngineCore/GUI/Button.h"

#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/GUI/FontRenderer.h"
#include "EngineCore/GUI/Font.h"
#include "EngineCore/GUI/Sprite.h"

#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Resources/ResourceManager.h"

#define SHIFT_TEXT_SYMBOL_X 20.5f
#define SHIFT_TEXT_SYMBOL_Y 8.f

namespace GUI
{
	Button::Button(Sprite* face, std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec2 pos, glm::vec2 scale,
		std::string text, std::string shaderName, std::shared_ptr<Font> font, glm::vec3 textColor)
		: GUI_element(std::move(pMaterial), "button")
		, m_textRenderer(std::make_unique<FontRenderer>(font, ResourceManager::getShaderProgram(shaderName)))
		, m_face(std::move(face))
		, m_color(textColor)
		, m_text(text)
	{
		m_position = pos;
		m_scale = scale;
		m_face->set_position(pos);
		m_face->set_scale(scale);
		m_pos_text = glm::vec2(pos.x - ((text.length() / 2) * SHIFT_TEXT_SYMBOL_X), pos.y - SHIFT_TEXT_SYMBOL_Y);
	}
	Button::~Button()
	{
		
	}
	void Button::on_render_prj(glm::mat4& prj)
	{
		m_textRenderer->render_text(m_text, m_pos_text.x, m_pos_text.y, 1.f, m_color, prj);
		m_face->on_render_prj(prj);
	}
}