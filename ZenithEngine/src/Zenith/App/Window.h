#pragma once

#include "Zenith/Utils/ZenithException.h"
#include "Zenith/Renderer/Graphics.h"
#include "Zenith/Input/Keyboard.h"
#include "Zenith/Input/Mouse.h"
#include "Zenith/Input/EventListener.h"
#include <Windows.h>

namespace Zenith
{
	class Graphics;

	class Window
	{
		friend class Win32WindowClass;
		friend class Input;
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
		Window(int width, int height, const std::string& title, bool fullScreen);
		~Window();

		void BindEventListener(EventListener* eventListener) noexcept;

		void SetWidth(int width);
		void SetHeight(int height);
		void SetTitle(const std::string& title);
		void SetFullscreenState(bool fullScreen);
		void Show();
		void Hide();

		int GetWidth() const noexcept;
		int GetHeight() const noexcept; 
		std::string GetTitle() const noexcept;
		Graphics* GetGfx() const noexcept;
		HWND GetHWND() const noexcept;

		void ProcessEvents();
		void CreateGraphicsContext(Graphics::API targetApi);

	private:
		int m_XPos, m_YPos;
		int m_Width, m_Height;
		std::string m_title;
		bool	m_IsFullscreen, 
				m_IsShown,
				m_HasFocus;
		Graphics* m_Graphics;
		Keyboard m_Keyboard;
		Mouse m_Mouse;
		HWND hWnd;
		EventListener* m_EventListener;

	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT CALLBACK HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	};
}
