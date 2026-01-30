#include "zenithpch.h"
#include "Font.h"
#include "Zenith/Utils/ZenithException.h"
#include "FontLoader.h"

namespace Zenith
{
	Font::Font(const std::string& path, unsigned int fontSize, Texture2D::Filter filterMode)
	{
		FontLoader::Face& face = FontLoader::LoadFace(path, fontSize);
		m_FontSize = 0u;

		for (unsigned char c = 0; c < 255; c++)
		{
			auto& glyph = face.glyphs[c];

			Character character = {
				{ glyph.bitmap.width, glyph.bitmap.height },
				{ glyph.bitmapLeft, glyph.bitmapTop },
				static_cast<uint32_t>(glyph.advance.x >> 6)
			};
			m_Characters.insert(std::pair<char, Character>(c, character));

			if (static_cast<unsigned int>(character.GlyphSize.y) > m_FontSize)
				m_FontSize = character.GlyphSize.y;
		}
	}

	Font::~Font()
	{
	}
}
