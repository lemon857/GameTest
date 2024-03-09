#include "EngineCore/GUI/TextRenderer.h"

#include "EngineCore/GUI/Font.h"
#include "EngineCore/Renderer/ShaderProgram.h"
#include "EngineCore/Renderer/Renderer.h"
#include "EngineCore/Renderer/VertexArray.h"
#include "EngineCore/Renderer/VertexBuffer.h"
#include "EngineCore/Renderer/VertexBufferLayout.h"

#include "EngineCore/System/ShadersSettings.h"
#include "EngineCore/System/SysFunc.h"

#include <glad/glad.h>
#include <locale>
#include <codecvt>

namespace GUI
{
    static unsigned int g_current_text_ID = 0;
	TextRenderer::TextRenderer(Font* font, RenderEngine::ShaderProgram* shader,
        std::string text, glm::vec3 color, glm::vec2 pos, glm::vec2 scale, std::string name, bool isCenterCoord)
		: m_font(std::move(font))
		, m_shader(std::move(shader))
        , m_color(color)
        , m_text(sysfunc::ctowstr(text))
        , m_isCenterCoords(isCenterCoord)
        , GUI_element(name == "default" ? "Text" + std::to_string(g_current_text_ID++) : name)
	{
        m_layer = 1;
        m_position_p = pos;
        m_scale_p = scale;

        RenderEngine::Renderer::setBlend(true);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_vertexArray = std::make_unique<RenderEngine::VertexArray>();
        m_vertexCoordsBuffer = new RenderEngine::VertexBuffer();

        m_vertexCoordsBuffer->init(NULL, sizeof(float) * 6 * 4, false);
		RenderEngine::VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(4, false);
		m_vertexArray->addBuffer(*m_vertexCoordsBuffer, vertexCoordsLayout);
	}
    TextRenderer::TextRenderer(Font* font, RenderEngine::ShaderProgram* shader, 
        std::wstring text, glm::vec3 color, glm::vec2 pos, glm::vec2 scale, std::string name, bool isCenterCoord)
        : m_font(std::move(font))
        , m_shader(std::move(shader))
        , m_color(color)
        , m_text(text)
        , m_isCenterCoords(isCenterCoord)
        , GUI_element(name == "default" ? "Text" + std::to_string(g_current_text_ID++) : name)
    {
        m_layer = 1;
        m_position_p = pos;
        m_scale_p = scale;

        RenderEngine::Renderer::setBlend(true);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_vertexArray = std::make_unique<RenderEngine::VertexArray>();
        m_vertexCoordsBuffer = new RenderEngine::VertexBuffer();

        m_vertexCoordsBuffer->init(NULL, sizeof(float) * 6 * 4, false);
        RenderEngine::VertexBufferLayout vertexCoordsLayout;
        vertexCoordsLayout.addElementLayoutFloat(4, false);
        m_vertexArray->addBuffer(*m_vertexCoordsBuffer, vertexCoordsLayout);
    }
	void TextRenderer::render_text(std::wstring text, float x, float y, float scale, glm::vec3 color, float layer, glm::mat4& prj)
	{ 
        // activate corresponding render state	
        m_shader->use();
        m_shader->setFloat(SS_LAYER_TEXT_PROP_NAME, layer);
        m_shader->setVec3(SS_COLOR_PROP_NAME, glm::vec3(color.x, color.y, color.z));
        m_shader->setMatrix4(SS_VIEW_PROJECTION_MATRIX_NAME, prj);
        glActiveTexture(GL_TEXTURE0);  // trash        
        m_vertexArray->bind();

        // iterate through all Characters1
        std::wstring::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Font_Glyph ch = m_font->get_glyph(*c);

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID); // trash
            // update content of VBO memory
            m_vertexCoordsBuffer->update(vertices, sizeof(vertices));
            // render quad 
            RenderEngine::Renderer::drawTriangles(*m_vertexArray);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        m_vertexArray->unbind();
        glBindTexture(GL_TEXTURE_2D, 0); // trash
	}
    void TextRenderer::on_render_prj(glm::mat4& prj)
    {
        if (!m_isActive) return;
        render_text(m_text, m_position.x, m_position.y, m_font->get_scale(), m_color, m_layer + 2, prj);
    }
    void TextRenderer::set_text(std::string text)
    {
        m_text = sysfunc::ctowstr(text);
        m_position = glm::vec2(m_isCenterCoords ? (m_p_pos.x - (((float)m_text.length() / 2.f) * SHIFT_TEXT_SYMBOL_X)) : m_p_pos.x, m_p_pos.y);
    }
    void TextRenderer::set_text(std::wstring text)
    {
        m_text = text;
        m_position = glm::vec2(m_isCenterCoords ? (m_p_pos.x - (((float)m_text.length() / 2.f) * SHIFT_TEXT_SYMBOL_X)) : m_p_pos.x, m_p_pos.y);
    }
    void TextRenderer::set_position(glm::vec2 pos)
    {
        m_p_pos = pos;
        //m_position = glm::vec2(m_isCenterCoords ? (m_p_pos.x - (((float)m_text.length() / 2.f) * SHIFT_TEXT_SYMBOL_X)) : m_p_pos.x, m_p_pos.y);
        m_position = glm::vec2(m_isCenterCoords ? (m_p_pos.x - (((float)m_text.length() / 2.f) * SHIFT_TEXT_SYMBOL_R_X)) : m_p_pos.x, m_p_pos.y);
    }
}