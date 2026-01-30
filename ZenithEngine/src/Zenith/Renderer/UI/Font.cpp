#include "zenithpch.h"
#include "Font.h"
#include "Zenith/Utils/ZenithException.h"
#include "FontLoader.h"
#include "Zenith/Platform/OpenGL/UI/OpenGLFont.h"

namespace Zenith
{
	Font* Font::Load(Graphics* gfx, const std::string& path, unsigned int fontSize, Texture2D::Filter filterMode)
	{
		switch (gfx->GetAPIType())
		{
		case Graphics::API::OpenGL:
			return new OpenGLFont(path, fontSize, filterMode);
		case Graphics::API::D3D11:
			// TODO: Implement for D3D11
			break;
		default:
			// TODO: Throw an exception
			break;
		}
	}

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
