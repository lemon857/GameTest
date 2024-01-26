#pragma once

#include <glm/vec2.hpp>
#include <string>
#include <map>

namespace GUI
{
	struct Font_Glyph {
		unsigned int TextureID; // ID текстуры глифа
		glm::ivec2   Size;      // Размеры глифа
		glm::ivec2   Bearing;   // Смещение верхней левой точки глифа
		unsigned int Advance;   // Горизонтальное смещение до начала следующего глифа
	};

	class Font
	{
	public:
		Font();
		~Font();

		bool load(std::string& font_path, unsigned int font_size);

		Font_Glyph get_glyph(const wchar_t sym);

		inline void set_scale(float scale) { m_scale = scale; }
		inline float get_scale() { return m_scale; }

	private:
		std::map<const wchar_t, Font_Glyph> m_glyphs;

		float m_scale;
	};
}