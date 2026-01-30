#include "zenithpch.h"
#include "Texture2D.h"
#include "Zenith/Utils/ZenithException.h"
#include "Zenith/Platform/OpenGL/OpenGLTexture2D.h"
#include "Zenith/Platform/DirectX/D3D11Texture2D.h"

namespace Zenith
{
	Texture2D* Texture2D::LoadWhiteTexture(Graphics* gfx)
	{
		unsigned char pixelData[] = { 255, 255, 255, 255 };
		switch (gfx->GetAPIType())
		{
#if ZENITH_PLATFORM_WINDOWS
		case Graphics::API::D3D11:
			return new D3D11Texture2D(gfx, pixelData, 1, 1, 4, 1, Filter::Point, Wrap::Clamp);
#endif
		case Graphics::API::OpenGL:
			return new OpenGLTexture2D(gfx, pixelData, 1, 1, 4, 1, Filter::Point, Wrap::Clamp);
		default:
			// TODO: Throw exception
			break;
		}
	}

	Texture2D* Texture2D::LoadFromFile(Graphics* gfx, const std::string& imageFile, unsigned int pixelPerUnit, bool forceRGBA, Filter filter, Wrap wrap)
	{
		int desiredChannelCount = forceRGBA ? STBI_rgb_alpha : STBI_default;

		int w, h, nChannels;

		if (gfx->GetAPIType() == Graphics::API::OpenGL)
			stbi_set_flip_vertically_on_load(true);

		unsigned char* pixels = stbi_load(imageFile.c_str(), &w, &h, &nChannels, desiredChannelCount);

		if (!pixels)
		{
			// Image not loaded
			throw ResourceNotFoundException(__LINE__, __FILE__, imageFile);
		}

		switch (gfx->GetAPIType())
		{
#if ZENITH_PLATFORM_WINDOWS
		case Graphics::API::D3D11:
			return new D3D11Texture2D(gfx, pixels, w, h, nChannels, pixelPerUnit, filter, wrap);
#endif
		case Graphics::API::OpenGL:
			return new OpenGLTexture2D(gfx, pixels, w, h, nChannels, pixelPerUnit, filter, wrap);
		default:
			// TODO: Throw an exception for unsupported API
			break;
		}

		stbi_image_free(pixels);
	}

	Texture2D* Texture2D::LoadFromMemory(Graphics* gfx, unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit, Filter filter, Wrap wrap)
	{
		switch (gfx->GetAPIType())
		{
#if ZENITH_PLATFORM_WINDOWS
		case Graphics::API::D3D11:
			return new D3D11Texture2D(gfx, pixels, width, height, channelCount, pixelPerUnit, filter, wrap);
#endif
		case Graphics::API::OpenGL:
			return new OpenGLTexture2D(gfx, pixels, width, height, channelCount, pixelPerUnit, filter, wrap);
		default:
			// TODO: Throw an exception for unsupported API
			break;
		}
	}

	Texture2D::Texture2D(Graphics* gfx, unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit, Filter filter, Wrap wrap)
		:
		m_Width(width), m_Height(height), m_ChannelCount(channelCount), m_PixelPerUnit(pixelPerUnit), m_Filter(filter), m_Wrap(wrap), m_Gfx(gfx)
	{
		GenerateTextureFromBytes(pixels, width, height, channelCount, filter, wrap);
	}

	Texture2D::~Texture2D()
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
	}
}
