#pragma once

#include "Zenith/Renderer/Bindable.h"
#include "D3D11Graphics.h"

#include <d3d11.h>

namespace Zenith
{
	class D3D11Bindable : public virtual Bindable
	{
	public:
		virtual void Bind(Graphics* gfx) noexcept override = 0;
		virtual ~D3D11Bindable() = default;
	protected:
		static ID3D11DeviceContext* GetContext(D3D11Graphics* gfx) noexcept;
		static ID3D11Device* GetDevice(D3D11Graphics* gfx) noexcept;
		static DxgiInfoManager& GetInfoManager(D3D11Graphics* gfx) noexcept (!_DEBUG);
	};
}
