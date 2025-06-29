#pragma once

#include "D3D11Graphics.h"

namespace Zenith
{
	class D3D11Renderer
	{
	protected:
		static ID3D11DeviceContext* GetContext(D3D11Graphics* gfx) noexcept;
		static ID3D11Device* GetDevice(D3D11Graphics* gfx) noexcept;
		static DxgiInfoManager& GetInfoManager(D3D11Graphics* gfx) noexcept (!_DEBUG);
	};
}
