#pragma once

#include <string>
#include <cstdint>
#include "Bindable.h"

namespace Zenith
{
	class Texture2D : public virtual Bindable
	{
	public:
		enum class Filter { Point = 0, Bilinear = 1, Trilinear = 2 };
		enum class Wrap { Repeat = 0, Clamp = 1 };

	public:
		static Texture2D* LoadWhiteTexture(Graphics* gfx);
		static Texture2D* LoadFromFile(Graphics* gfx, const std::string& imageFile, unsigned int pixelPerUnit = 100u, bool forceRGBA = false, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);
		static Texture2D* LoadFromMemory(Graphics* gfx, unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit = 100u, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);

	protected:
		Texture2D(Graphics* gfx, unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit = 100u, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);
		Texture2D(Graphics* gfx, const std::string& imageFile, unsigned int pixelPerUnit = 100u, bool forceRGBA = false, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);

	public:
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		virtual ~Texture2D();

		virtual void Bind(Graphics* gfx) noexcept override = 0;

		int GetWidth() const noexcept;
		int GetHeight() const noexcept;
		int ChannelCount() const noexcept;
		unsigned int PixelPerUnit() const noexcept;
		Filter GetFilterMode() const noexcept;
		Wrap GetWrapMode() const noexcept;

		void SetPixelPerUnit(int ppu) noexcept;
		virtual void ChangeFilterMode(Filter f);
		virtual void ChangeWrapMode(Wrap w);

	protected:
		virtual void GenerateTextureFromBytes(unsigned char* pixels, int w, int h, int ch, Filter f, Wrap wrp);

	protected:
		int				m_Width,
						m_Height,
						m_ChannelCount;
		unsigned int	m_PixelPerUnit;
		Filter			m_Filter;
		Wrap			m_Wrap;
		Graphics*		m_Gfx;
	};
}
