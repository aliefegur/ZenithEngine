#pragma once

#include <map>
#include <string>
#include <glm/vec2.hpp>

#include "../Texture2D.h"

namespace Zenith
{
	class Font
	{
		friend class UIRenderer;
	public:
		struct Character
		{
			glm::ivec2	GlyphSize;
			glm::ivec2	Bearing;	// Offset from the baseline to left-top of the glyph
			uint32_t	Advance;	// Horizontal offset to advance	to next glyph
		};

		Font(const std::string& path, unsigned int fontSize = 64, Texture2D::Filter filterMode = Texture2D::Filter::Bilinear);
		virtual ~Font();

	protected:
		std::map<unsigned char, Character> m_Characters;
		unsigned int m_FontSize;
	};
}
