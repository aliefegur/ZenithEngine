#if ZENITH_PLATFORM_WINDOWS

#pragma once

#include "Zenith/App/Window.h"
#include <Windows.h>

namespace Zenith
{
	class Win32Window : public Window
	{
		friend class Win32WindowClass;
	public:
		class Exception : public ZenithException
		{
			using ZenithException::ZenithException;
		public:
			static std::string TranslateErrorCode(HRESULT hr) noexcept;
		};

		class HrException : public Exception
		{
		public:
			HrException(int line, const char* file, HRESULT hr) noexcept;
			virtual const char* what() const noexcept override;
			virtual const char* GetType() const noexcept override;
			HRESULT	GetErrorCode() const noexcept;
			std::string GetErrorDescription() const noexcept;
		private:
			HRESULT m_Hr;
		};

	public:
		Win32Window(int width, int height, const std::string& title, bool fullScreen);
		~Win32Window();

		void SetWidth(int width) override;
		void SetHeight(int height) override;
		void SetTitle(const std::string& title) override;
		void SetFullscreenState(bool fullScreen) override;
		void Show() override;
		void Hide() override;

		void ProcessEvents() override;

		HWND GetHWND() const noexcept;

	private:
		HWND hWnd;

	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT CALLBACK HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	};
}

#endif
