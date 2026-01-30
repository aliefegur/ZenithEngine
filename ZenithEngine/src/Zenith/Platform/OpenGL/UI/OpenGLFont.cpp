#include "zenithpch.h"
#include "OpenGLFont.h"
#include "Zenith/Utils/ZenithException.h"
#include "Zenith/Renderer/UI/FontLoader.h"

namespace Zenith
{
	OpenGLFont::OpenGLFont(const std::string& path, unsigned int fontSize, Texture2D::Filter filterMode)
		: Font(path, fontSize, filterMode)
	{
		FontLoader::Face& face = FontLoader::LoadFace(path, fontSize);	

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (unsigned char c = 0; c < 255; c++)
		{
			auto& glyph = face.glyphs[c];

			// Generate texture
			uint32_t texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph.bitmap.width, glyph.bitmap.height, 0, GL_RED, GL_UNSIGNED_BYTE, glyph.bitmap.buffer);
			
			// Set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			switch (filterMode)
			{
			case Zenith::Texture2D::Filter::Point:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			case Zenith::Texture2D::Filter::Trilinear:
			case Zenith::Texture2D::Filter::Bilinear:
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			}

			// Unbind texture
			glBindTexture(GL_TEXTURE_2D, 0);

			// Insert into map
			m_TextureIDs.insert(std::pair<unsigned char, uint32_t>(c, texture));
		}
	}

	OpenGLFont::~OpenGLFont()
	{
		for (auto& [ch, tex] : m_TextureIDs)
		{
			glDeleteTextures(1, &tex);
		}
	}
}
