#include "zenithpch.h"
#include "D3D11Graphics.h"
#include "Zenith/App/Window.h"
#include <wrl.h>

namespace wrl = Microsoft::WRL;

namespace Zenith
{
	D3D11Graphics::D3D11Graphics(Window& targetWindow) : Graphics(targetWindow)
	{
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2;
		sd.OutputWindow = targetWindow.GetHWND();
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = NULL;

		// For checking D3D function results
		HRESULT hr;

		UINT swapCreateFlags = NULL;
#ifndef NDEBUG
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// Create device & swap chain
		GFX_THROW(D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			swapCreateFlags,
			nullptr,
			NULL,
			D3D11_SDK_VERSION,
			&sd,
			&pSwap,
			&pDevice,
			nullptr,
			&pContext
		));

		// Get backbuffer & make it render target
		wrl::ComPtr<ID3D11Resource> pBackBuffer;
		GFX_THROW(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		GFX_THROW(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));

		// Create depth stencil state
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		wrl::ComPtr<ID3D11DepthStencilState> pDSState;
		GFX_THROW(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));
		
		// Bind depth state
		pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

		// Create depth stencil texture
		wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = targetWindow.GetWidth();
		descDepth.Height = targetWindow.GetHeight();
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		GFX_THROW(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

		// Create view of depth stencil texture
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0u;
		GFX_THROW(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV));

		// Bind depth stencil view to OM
		pContext->OMSetRenderTargets(1u, &pTarget, pDSV);

		// Create initial viewport
		D3D11_VIEWPORT vp = {};
		vp.TopLeftX = static_cast<float>(0);
		vp.TopLeftY = static_cast<float>(0);
		vp.Width = static_cast<float>(targetWindow.GetWidth());
		vp.Height = static_cast<float>(targetWindow.GetHeight());
		// TODO: Check out these values
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		pContext->RSSetViewports(1, &vp);

		m_CurrentAPI = Graphics::API::D3D11;
	}
	
	D3D11Graphics::~D3D11Graphics()
	{
		pDevice->Release();
		pSwap->Release();
		pContext->Release();
		pTarget->Release();
		pDSV->Release();
	}

	void D3D11Graphics::EndFrame()
	{
		HRESULT hr;

		if (FAILED(hr = pSwap->Present(1u, NULL)))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
			{
				throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
			}
			else
			{
				throw GFX_EXCEPT(hr);
			}
		}
	}
	
	void D3D11Graphics::ClearBuffer(float red, float green, float blue, float alpha) noexcept
	{
		const float color[] = { red, green, blue, alpha };
		pContext->ClearRenderTargetView(pTarget, color);
		pContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	void D3D11Graphics::DrawIndexed(unsigned int count)
	{
		pContext->OMSetRenderTargets(1, &pTarget, pDSV);
		pContext->DrawIndexed(count, 0, 0);
	}

	void D3D11Graphics::ApplyViewport(const Viewport& viewport)
	{
		HRESULT hr;

		// Release resources
		if (pTarget) { pTarget->Release(); pTarget = nullptr; }
		if (pDSV) { pDSV->Release();    pDSV = nullptr; }

		// Swap chain resize
		GFX_THROW(pSwap->ResizeBuffers(0, viewport.GetW(), viewport.GetH(), DXGI_FORMAT_R8G8B8A8_UNORM, NULL));

		// Recreate render target view
		ID3D11Texture2D* pBackBuffer = nullptr;
		GFX_THROW(pSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer)));
		GFX_THROW(pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTarget));
		pBackBuffer->Release();

		// Recreate depth stencil view
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = viewport.GetW();
		descDepth.Height = viewport.GetH();
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		ID3D11Texture2D* pDepthStencil = nullptr;
		GFX_THROW(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		GFX_THROW(pDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDSV));
		pDepthStencil->Release();

		// Update viewport
		D3D11_VIEWPORT vp = {};
		vp.TopLeftX = static_cast<float>(viewport.GetX());
		vp.TopLeftY = static_cast<float>(viewport.GetY());
		vp.Width = static_cast<float>(viewport.GetW());
		vp.Height = static_cast<float>(viewport.GetH());
		// TODO: Check out these values
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		pContext->RSSetViewports(1, &vp);
	}

#pragma region HrException
	D3D11Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoList) noexcept
		:
		ZenithException(line, file), hr(hr)
	{
		for (const auto& m : infoList)
		{
			info += m;
			info.push_back('\n');
		}

		if (!info.empty())
		{
			info.pop_back();
		}
	}

	const char* D3D11Graphics::HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl;
		if (!info.empty())
		{
			oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
		}
		oss << GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	const char* D3D11Graphics::HrException::GetType() const noexcept
	{
		return "D3D11 Graphics Exception";
	}

	HRESULT D3D11Graphics::HrException::GetErrorCode() const noexcept
	{
		return hr;
	}

	std::string D3D11Graphics::HrException::GetErrorDescription() const noexcept
	{
		char* pMsgBuf = nullptr;
		DWORD nMsgLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
		);

		if (nMsgLen == 0) return "Unidentified Error Code";

		std::string errorStirng = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorStirng;
	}

	std::string D3D11Graphics::HrException::GetErrorInfo() const noexcept
	{
		return info;
	}
#pragma endregion

#pragma region InfoException
	D3D11Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoList)
		:
		ZenithException(line, file)
	{
		for (const auto& i : infoList)
		{
			info += i;
			info.push_back('\n');
		}

		if (!info.empty()) info.pop_back();
	}

	const char* D3D11Graphics::InfoException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl
			<< GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	const char* D3D11Graphics::InfoException::GetType() const noexcept
	{
		return "D3D11 Graphics Info Exception";
	}

	std::string D3D11Graphics::InfoException::GetErrorInfo() const noexcept
	{
		return info;
	}
#pragma endregion

#pragma region Device Removed Exception
	const char* D3D11Graphics::DeviceRemovedException::GetType() const noexcept
	{
		return "D3D11 Graphics Device Removed Exception";
	}
#pragma endregion
}
