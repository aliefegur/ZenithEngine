#pragma once

#include "Zenith/Renderer/Texture2D.h"
#include "D3D11Bindable.h"

namespace Zenith
{
	class D3D11Texture2D : public Texture2D, public D3D11Bindable
	{
	public:
		D3D11Texture2D(Graphics* gfx, unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit = 100u, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);
		D3D11Texture2D(Graphics* gfx, const std::string& imageFile, unsigned int pixelPerUnit = 100u, bool forceRGBA = false, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);
		~D3D11Texture2D();

		void Bind(Graphics* gfx) noexcept override;

		void ChangeFilterMode(Filter f) override;
		void ChangeWrapMode(Wrap w) override;

		ID3D11ShaderResourceView* GetShaderResourceView() const noexcept;
		ID3D11SamplerState* GetSamplerState() const noexcept;

	private:
		void GenerateTextureFromBytes(unsigned char* pixels, int w, int h, int ch, Filter f, Wrap wrp) override;

	private:
		ID3D11ShaderResourceView* pTextureView = nullptr;
		ID3D11SamplerState* pSampler = nullptr;

	private:
		static constexpr D3D11_FILTER filters[] = {
			D3D11_FILTER_MIN_MAG_MIP_POINT,
			D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT,
			D3D11_FILTER_MIN_MAG_MIP_LINEAR
		};

		static constexpr D3D11_TEXTURE_ADDRESS_MODE textureAddresses[] = {
			D3D11_TEXTURE_ADDRESS_WRAP,
			D3D11_TEXTURE_ADDRESS_CLAMP
		};
	};
}
