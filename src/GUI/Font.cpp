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

	bool Font::load(std::string& font_path, unsigned int font_size)
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
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
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
			m_glyphs.insert(std::pair<char, Font_Glyph>(c, character));
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
		return true;
	}

	Font_Glyph Font::get_glyph(const char sym)
	{
		return m_glyphs[sym];
	}
}