#include "zenithpch.h"
#include "D3D11Bindable.h"

namespace Zenith
{
	ID3D11DeviceContext* D3D11Bindable::GetContext(D3D11Graphics* gfx) noexcept
	{
		return gfx->pContext;
	}

	ID3D11Device* D3D11Bindable::GetDevice(D3D11Graphics* gfx) noexcept 
	{
		return gfx->pDevice;
	}
	
	DxgiInfoManager& D3D11Bindable::GetInfoManager(D3D11Graphics* gfx) noexcept(!_DEBUG)
	{
		return gfx->m_InfoManager;
	}
}
