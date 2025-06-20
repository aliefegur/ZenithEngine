#include "zenithpch.h"
#include "OpenGLTexture2D.h"

namespace Zenith
{
	Zenith::OpenGLTexture2D::OpenGLTexture2D(Graphics* gfx, unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit, Filter filter, Wrap wrap)
		:
		Texture2D(gfx, pixels, width, height, channelCount, pixelPerUnit, filter, wrap)
	{
	}
	
	OpenGLTexture2D::OpenGLTexture2D(Graphics* gfx, const std::string& imageFile, unsigned int pixelPerUnit, bool forceRGBA, Filter filter, Wrap wrap)
		:
		Texture2D(gfx, imageFile, pixelPerUnit, forceRGBA, filter, wrap)
	{
	}
	
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}
	
	void OpenGLTexture2D::Bind(Graphics* gfx) noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	uint32_t OpenGLTexture2D::GetID() const noexcept
	{
		return m_TextureID;
	}
	
	void OpenGLTexture2D::ChangeFilterMode(Filter f)
	{
		Texture2D::ChangeFilterMode(f);

		constexpr GLint filterIDs[] = { GL_NEAREST, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR };

		Bind(m_Gfx);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterIDs[static_cast<int>(f)]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterIDs[static_cast<int>(f)]);
	}
	
	void OpenGLTexture2D::ChangeWrapMode(Wrap w)
	{
		Texture2D::ChangeWrapMode(w);

		constexpr GLint wrapIDs[] = { GL_REPEAT, GL_CLAMP_TO_EDGE };

		Bind(m_Gfx);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapIDs[static_cast<int>(w)]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapIDs[static_cast<int>(w)]);
	}
	
	void OpenGLTexture2D::GenerateTextureFromBytes(unsigned char* pixels, int w, int h, int ch, Filter f, Wrap wrp)
	{
		constexpr GLint imgFormats[] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
		GLint imgFormat = NULL;

		if (ch >= 1 && ch <= 4) imgFormat = imgFormats[ch - 1];
		else std::cout << "Unsupported image format! (" << ch << " channels)" << std::endl;	// TODO: Throw an exception

		glGenTextures(1, &m_TextureID);
		Bind(m_Gfx);
		glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, w, h, NULL, imgFormat, GL_UNSIGNED_BYTE, pixels);
		ChangeFilterMode(f);
		ChangeWrapMode(wrp);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}
