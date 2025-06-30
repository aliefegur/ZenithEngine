#include "zenithpch.h"
#include "D3D11Texture2D.h"

namespace Zenith
{
	Zenith::D3D11Texture2D::D3D11Texture2D(Graphics* gfx, unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit, Filter filter, Wrap wrap)
		:
		Texture2D(gfx, pixels, width, height, channelCount, pixelPerUnit, filter, wrap)
	{
	}
	
	D3D11Texture2D::D3D11Texture2D(Graphics* gfx, const std::string& imageFile, unsigned int pixelPerUnit, bool forceRGBA, Filter filter, Wrap wrap)
		:
		Texture2D(gfx, imageFile, pixelPerUnit, forceRGBA, filter, wrap)
	{
	}

	D3D11Texture2D::~D3D11Texture2D()
	{
		pTextureView->Release();
		pSampler->Release();
	}

	void D3D11Texture2D::Bind(Graphics* gfx_) noexcept
	{
		D3D11Graphics* gfx = reinterpret_cast<D3D11Graphics*>(gfx_);
		
		GetContext(gfx)->PSSetShaderResources(0, 1, &pTextureView);
		GetContext(gfx)->PSSetSamplers(0, 1, &pSampler);
	}

	void D3D11Texture2D::ChangeFilterMode(Filter f)
	{		
		Texture2D::ChangeFilterMode(f);

		// Obtain D3D11 Graphics and info manager
		D3D11Graphics* gfx = reinterpret_cast<D3D11Graphics*>(m_Gfx);
		INFO_MANAGER(gfx);

		// Release last sampler
		pSampler->Release();

		// Create new sampler desc
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = filters[static_cast<int>(f)];
		samplerDesc.AddressU = textureAddresses[static_cast<int>(m_Wrap)];
		samplerDesc.AddressV = textureAddresses[static_cast<int>(m_Wrap)];
		samplerDesc.AddressW = textureAddresses[static_cast<int>(m_Wrap)];

		// Create new sampler
		GFX_THROW(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
	}

	void D3D11Texture2D::ChangeWrapMode(Wrap w)
	{
		Texture2D::ChangeWrapMode(w);

		// Obtain D3D11 Graphics and info manager
		D3D11Graphics* gfx = reinterpret_cast<D3D11Graphics*>(m_Gfx);
		INFO_MANAGER(gfx);

		// Release last sampler
		pSampler->Release();

		// Create new sampler desc
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = filters[static_cast<int>(m_Filter)];
		samplerDesc.AddressU = textureAddresses[static_cast<int>(w)];
		samplerDesc.AddressV = textureAddresses[static_cast<int>(w)];
		samplerDesc.AddressW = textureAddresses[static_cast<int>(w)];

		// Create new sampler
		GFX_THROW(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
	}

	ID3D11ShaderResourceView* D3D11Texture2D::GetShaderResourceView() const noexcept
	{
		return pTextureView;
	}

	ID3D11SamplerState* D3D11Texture2D::GetSamplerState() const noexcept
	{
		return pSampler;
	}

	void D3D11Texture2D::GenerateTextureFromBytes(unsigned char* pixels, int w, int h, int ch, Filter f, Wrap wrp)
	{
		D3D11Graphics* gfx = reinterpret_cast<D3D11Graphics*>(m_Gfx);
		INFO_MANAGER(gfx);

		constexpr DXGI_FORMAT textureFormats[] = {
			DXGI_FORMAT_R8_UNORM,
			DXGI_FORMAT_R8G8_UNORM,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_R8G8B8A8_UNORM,
		};

		// Texture descriptor
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = w;
		textureDesc.Height = h;
		textureDesc.Format = textureFormats[ch - 1];
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = NULL;
		textureDesc.MiscFlags = NULL;

		// Subresource data
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = pixels;
		sd.SysMemPitch = w * ch * static_cast<int>(sizeof(unsigned char));

		// Create texture
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW(GetDevice(gfx)->CreateTexture2D(&textureDesc, &sd, &pTexture));

		// Create resource view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		GFX_THROW(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView));
	}
}
