#include "EngineCore/GUI/Button.h"

#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/GUI/FontRenderer.h"
#include "EngineCore/GUI/Font.h"

#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Resources/ResourceManager.h"

namespace GUI
{
	Button::Button(GUI_element face, std::shared_ptr<RenderEngine::Material> pMaterial, glm::vec2 pos, glm::vec2 scale, 
		std::string text, std::string shaderName, std::shared_ptr<Font> font)
		: GUI_element(std::move(pMaterial), "button")
		, m_textRenderer(std::make_unique<FontRenderer>(font, ResourceManager::getShaderProgram(shaderName)))
	{
	}
	Button::~Button()
	{
		
	}
	void Button::on_render()
	{
	}
}