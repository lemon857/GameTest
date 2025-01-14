#include "EngineCore/GUI/Font.h"

#include "EngineCore/System/Log.h"

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace GUI
{
	Font::Font()
	{
		
	}

	Font::~Font()
	{
		for (size_t i = 0; i < m_glyphs.size(); i++)
		{
			glDeleteTextures(1, &m_glyphs[i].TextureID);
		}
	}

	bool Font::load(std::string font_path, unsigned int font_size)
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			LOG_ERROR("FREETYPE: Could not init FreeType Library");
			return false;
		}

		FT_Face face;
		if (FT_New_Face(ft, font_path.c_str(), 0, &face))
		{
			LOG_ERROR("FREETYPE: Failed to load font");
			return false;
		}

		FT_Select_Charmap(face, ft_encoding_unicode);
		FT_Set_Pixel_Sizes(face, 0, font_size);
		
		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (wchar_t c = 0; c < face->num_glyphs; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				LOG_WARN("Error load glyph: {0}", c);
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Font_Glyph character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			m_glyphs.insert(std::pair<wchar_t, Font_Glyph>(c, character));
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		
		m_scale = 1.f;

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
		return true;
	}

	Font_Glyph Font::get_glyph(const wchar_t sym)
	{
		return m_glyphs[sym];
	}
	void Font::bind_font_texture()
	{
		glActiveTexture(GL_TEXTURE0);
	}
	void Font::bind_glyph_texture(const Font_Glyph sym)
	{
		glBindTexture(GL_TEXTURE_2D, sym.TextureID);
	}
	void Font::unbind_font()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
