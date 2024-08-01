#pragma once

#include <glm/vec2.hpp>
#include <string>
#include <map>

namespace GUI
{
	struct Font_Glyph {
		unsigned int TextureID; // ID òåêñòóðû ãëèôà
		glm::ivec2   Size;      // Ðàçìåðû ãëèôà
		glm::ivec2   Bearing;   // Ñìåùåíèå âåðõíåé ëåâîé òî÷êè ãëèôà
		unsigned int Advance;   // Ãîðèçîíòàëüíîå ñìåùåíèå äî íà÷àëà ñëåäóþùåãî ãëèôà
	};

	class Font
	{
	public:
		Font();
		~Font();

		bool load(std::string font_path, unsigned int font_size);

		Font_Glyph get_glyph(const wchar_t sym);

		inline void set_scale(float scale) { m_scale = scale; }
		inline float get_scale() { return m_scale; }

		void bind_font_texture();
		void bind_glyph_texture(const Font_Glyph sym);
		void unbind_font();

	private:
		std::map<const wchar_t, Font_Glyph> m_glyphs;

		float m_scale;
	};
}
