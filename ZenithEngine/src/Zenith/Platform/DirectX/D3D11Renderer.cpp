#include "zenithpch.h"
#include "D3D11Renderer.h"

namespace Zenith
{
	ID3D11DeviceContext* D3D11Renderer::GetContext(D3D11Graphics* gfx) noexcept
	{
		return gfx->pContext;
	}

	ID3D11Device* D3D11Renderer::GetDevice(D3D11Graphics* gfx) noexcept
	{
		return gfx->pDevice;
	}

#if ZENITH_DEBUG
	DxgiInfoManager& D3D11Renderer::GetInfoManager(D3D11Graphics* gfx) noexcept(!ZENITH_DEBUG)
	{
		return gfx->infoManager;
	}
#endif
}
