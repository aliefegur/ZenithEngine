#pragma once

#include <Zenith/Renderer/Texture2D.h>

namespace Zenith
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(Graphics* gfx, unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit = 100u, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);
		OpenGLTexture2D(Graphics* gfx, const std::string& imageFile, unsigned int pixelPerUnit = 100u, bool forceRGBA = false, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);
		~OpenGLTexture2D();

		void Bind(Graphics* gfx) override;
		uint32_t GetID() const noexcept;

		void ChangeFilterMode(Filter f) override;
		void ChangeWrapMode(Wrap w) override;

	private:
		void GenerateTextureFromBytes(unsigned char* pixels, int w, int h, int ch, Filter f, Wrap wrp) override;

	private:
		uint32_t m_TextureID = 0;
	};
}
