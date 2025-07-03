#pragma once

#include "Zenith/API.h"	
#include "Zenith/Renderer/Graphics.h"
#include "Zenith/Utils/ZenithException.h"
#include "DxgiInfoManager.h"
#include "Zenith/Renderer/Viewport.h"
#include <d3d11.h>

namespace Zenith
{
	class D3D11Graphics : public Graphics
	{
		friend class D3D11Bindable;
		friend class D3D11Renderer;
	public:
#pragma region Exceptions
		class HrException : public ZenithException
		{
		public:
			HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoList = {}) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorDescription() const noexcept;
			std::string GetErrorInfo() const noexcept;
		private:
			HRESULT hr;
			std::string info;
		};

		class InfoException : public ZenithException
		{
		public:
			InfoException(int line, const char* file, std::vector<std::string> info);
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			std::string GetErrorInfo() const noexcept;
		private:
			std::string info;
		};

		class DeviceRemovedException : public HrException
		{
			using HrException::HrException;
		public:
			const char* GetType() const noexcept override;
		private:
			std::string reason;
		};
#pragma endregion

	public:
		D3D11Graphics(Window& targetWindow);
		~D3D11Graphics();

		void EndFrame() override;
		void ClearBuffer(float red, float green, float blue, float alpha) noexcept override;
		void DrawIndexed(unsigned int count) override;
		
		void ApplyViewport(const Viewport& viewport);
		
	protected:
		void InitializeImGui() override;
		void NewImGuiFrame() override;
		void RenderImGui() override;
		void ShutdownImGui() override;

	private:
		ID3D11Device*			pDevice;
		IDXGISwapChain*			pSwap;
		ID3D11DeviceContext*	pContext;
		ID3D11RenderTargetView* pTarget;
		ID3D11DepthStencilView* pDSV;
		
#if ZENITH_DEBUG
		DxgiInfoManager infoManager;
#endif
	};
}

// Preprocessor macros for error handling for graphical operations
#define GFX_EXCEPT_NOINFO(hr) D3D11Graphics::HrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NOINFO(hrcall) if (FAILED(hr = (hrcall))) throw GFX_EXCEPT_NOINFO(hr)
#ifndef NDEBUG
#define GFX_EXCEPT(hr) D3D11Graphics::HrException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_DEVICE_REMOVED_EXCEPT(hr) D3D11Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call) infoManager.Set(); (call); { auto v = infoManager.GetMessages(); if (!v.empty()) {throw D3D11Graphics::InfoException(__LINE__, __FILE__, v);} }
#else
#define GFX_EXCEPT(hr) GFX_EXCEPT_NOINFO((hr))
#define GFX_DEVICE_REMOVED_EXCEPT(hr) D3D11Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr))
#define GFX_THROW_INFO_ONLY(call) (call)
#endif
#define GFX_THROW(hrcall) if (FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)

#ifdef NDEBUG
#define INFO_MANAGER(gfx) HRESULT hr
#else
#define INFO_MANAGER(gfx) HRESULT hr; DxgiInfoManager& infoManager = GetInfoManager((gfx))
#endif
