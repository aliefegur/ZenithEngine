#include "zenithpch.h"
#include "Texture2D.h"
#include "Zenith/Utils/ZenithException.h"

namespace Zenith
{
	Texture2D* Texture2D::LoadWhiteTexture(Graphics* gfx)
	{
		unsigned char pixelData[] = { 255, 255, 255 };
		switch (gfx->GetAPIType())
		{
		case Graphics::API::D3D11:
			break;
		case Graphics::API::OpenGL:
			return new Texture2D(gfx, pixelData, 1, 1, 3, 1, Filter::Point, Wrap::Clamp);
			break;
		default:
			// TODO: Throw exception
			break;
		}
	}

	Texture2D::Texture2D(Graphics* gfx, unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit, Filter filter, Wrap wrap)
		:
		m_Width(width), m_Height(height), m_ChannelCount(channelCount), m_PixelPerUnit(pixelPerUnit), m_Filter(filter), m_Wrap(wrap), m_Gfx(gfx)
	{
		GenerateTextureFromBytes(pixels, width, height, channelCount, filter, wrap);
	}

	Texture2D::Texture2D(Graphics* gfx, const std::string& imageFile, unsigned int pixelPerUnit, bool forceRGBA, Filter filter, Wrap wrap)
		:
		m_Gfx(gfx)
	{
		int desiredChannelCount = forceRGBA ? STBI_rgb_alpha : STBI_default;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* pixels = stbi_load(imageFile.c_str(), &m_Width, &m_Height, &m_ChannelCount, desiredChannelCount);

		if (!pixels)
		{
			// Image not loaded
			throw ResourceNotFoundException(__LINE__, __FILE__, imageFile);
		}

		GenerateTextureFromBytes(pixels, m_Width, m_Height, m_ChannelCount, filter, wrap);

		stbi_image_free(pixels);
		m_PixelPerUnit = pixelPerUnit;
	}

	Texture2D::~Texture2D()
	{	
	}

	void Texture2D::Bind(Graphics* gfx)
	{
	}

	int Texture2D::GetWidth() const noexcept
	{
		return m_Width;
	}
	int Texture2D::GetHeight() const noexcept
	{
		return m_Height;
	}
	int Texture2D::ChannelCount() const noexcept
	{
		return m_ChannelCount;
	}
	unsigned int Texture2D::PixelPerUnit() const noexcept
	{
		return m_PixelPerUnit;
	}
	Texture2D::Filter Texture2D::GetFilterMode() const noexcept
	{
		return m_Filter;
	}
	Texture2D::Wrap Texture2D::GetWrapMode() const noexcept
	{
		return m_Wrap;
	}
	

	void Texture2D::SetPixelPerUnit(int ppu) noexcept
	{
		m_PixelPerUnit = ppu;
	}
	void Texture2D::ChangeFilterMode(Filter f)
	{			
		m_Filter = f;
	}
	void Texture2D::ChangeWrapMode(Wrap w)
	{
		m_Wrap = w;
	}

	void Texture2D::GenerateTextureFromBytes(unsigned char* pixels, int w, int h, int ch, Filter f, Wrap wrp)
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
