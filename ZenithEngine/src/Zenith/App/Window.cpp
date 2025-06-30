#include "zenithpch.h"

#include "Window.h"
#include "Zenith/App/AppManager.h"
#include "Zenith/Renderer/Graphics.h"
#include "Zenith/Platform/OpenGL/OpenGLGraphics.h"
#include "Zenith/Platform/DirectX/D3D11Graphics.h"

#include <imgui.h>
#include <imgui_impl_win32.h>

#define	ZENITH_WND_EXCEPT(hr) Zenith::Window::HrException(__LINE__, __FILE__, hr)
#define ZENITH_WND_LASTEXCEPT() Zenith::Window::HrException(__LINE__, __FILE__, GetLastError())
#define LISTENER if (m_EventListener) m_EventListener

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Zenith
{
#pragma region Win32WindowClass
	class Win32WindowClass
	{
	public:
		static LPCSTR GetName() noexcept
		{
			return s_WndClassName;
		}
		static HINSTANCE GetInstance() noexcept
		{
			return s_WndClass.m_hInst;
		}

	private:
		Win32WindowClass() noexcept
			:
			m_hInst(GetModuleHandle(nullptr))
		{
			WNDCLASSEX wc = { 0 };
			wc.cbSize = sizeof(wc);
			wc.style = CS_OWNDC;
			wc.lpfnWndProc = Window::HandleMsgSetup;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = GetInstance();
			wc.hIcon = LoadIcon(GetInstance(), MAKEINTRESOURCE(101));
			wc.hIconSm = nullptr;
			wc.hCursor = nullptr;
			wc.hbrBackground = nullptr;
			wc.lpszMenuName = nullptr;
			wc.lpszClassName = GetName();

			RegisterClassEx(&wc);
		}

		~Win32WindowClass()
		{
			UnregisterClass(GetName(), GetInstance());
		}

	private:
		static constexpr LPCSTR s_WndClassName = "__ZenithEngineWin32WindowClass__";
		static Win32WindowClass s_WndClass;
		HINSTANCE m_hInst;
	};

	Win32WindowClass Win32WindowClass::s_WndClass = Win32WindowClass();
#pragma endregion

#pragma region Window
	Window::Window(int width, int height, const std::string& title, bool fullScreen)
		:
		m_Width(width), m_Height(height), m_title(title), m_IsFullscreen(fullScreen), m_Graphics(nullptr), m_HasFocus(false), m_IsShown(false), m_EventListener(nullptr)
	{
		constexpr DWORD windowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_SYSMENU | CS_OWNDC;

		// Calculate window size based on desired client region size
		RECT wr;
		wr.left = 100;
		wr.right = width + wr.left;
		wr.top = 100;
		wr.bottom = height + wr.top;

		if (AdjustWindowRect(&wr, windowStyle, FALSE) == 0)
		{
			throw ZENITH_WND_LASTEXCEPT();
		}

		// Window creation
		hWnd = CreateWindow(
			Win32WindowClass::GetName(),
			title.c_str(),
			windowStyle,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			nullptr, nullptr,
			Win32WindowClass::GetInstance(),
			this
		);

		if (hWnd == nullptr)
		{
			throw ZENITH_WND_LASTEXCEPT();
		}
	}

	Window::~Window()
	{
		DestroyWindow(hWnd);
	}

	void Window::BindEventListener(EventListener* eventListener) noexcept
	{
		m_EventListener = eventListener;
	}

	void Window::SetWidth(int width)
	{
		// TODO: Implement this
	}
	void Window::SetHeight(int height)
	{
		// TODO: Implement this
	}
	void Window::SetTitle(const std::string& title)
	{
		if (SetWindowText(hWnd, title.c_str()) == 0)
		{
			throw ZENITH_WND_LASTEXCEPT();
		}
	}
	void Window::SetFullscreenState(bool fullScreen)
	{
		// TODO: Implement this
	}
	void Window::Show()
	{
		ShowWindow(hWnd, SW_SHOWDEFAULT);
		m_IsShown = true;
	}
	void Window::Hide() 
	{
		if (ShowWindow(hWnd, SW_HIDE) == 0) 
		{
			throw ZENITH_WND_LASTEXCEPT();
		}
		m_IsShown = false;
	}
	int Window::GetWidth() const noexcept
	{
		return m_Width;
	}
	int Window::GetHeight() const noexcept
	{
		return m_Height;
	}
	std::string Window::GetTitle() const noexcept
	{
		return m_title;
	}
	Graphics* Window::GetGfx() const noexcept
	{
		return m_Graphics;
	}
	HWND Window::GetHWND() const noexcept
	{
		return hWnd;
	}

	void Window::ProcessEvents()
	{
		m_Keyboard.Flush();

		MSG msg;

		while (PeekMessage(&msg, hWnd, NULL, NULL, PM_REMOVE) > 0)
		{
			if (msg.message == WM_QUIT)
			{
				AppManager::QuitApplication(static_cast<int>(msg.wParam));
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::CreateGraphicsContext(Graphics::API targetApi)
	{
		switch (targetApi)
		{
		case Zenith::Graphics::API::D3D11: 
			m_Graphics = new D3D11Graphics(*this);
			break;
		case Zenith::Graphics::API::OpenGL:
			m_Graphics = new OpenGLGraphics(*this);
			break;
		case Zenith::Graphics::API::None:
			// TODO: Throw error
			m_Graphics = nullptr;
			break;
		default:
			// TODO: Throw error
			m_Graphics = nullptr;
			break;
		}
	}

	LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::HandleMsgThunk));
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		{
			return true;
		}

		// TODO: Implement Mouse, Scroll, Window Move, Minimize, Maximize, etc. events
		switch (msg)
		{
		case WM_CLOSE:	// Window closed
			m_IsShown = false;
			LISTENER->OnWindowClose(this);
			AppManager::QuitApplication(0);
			break;

		case WM_SETFOCUS:	// Focus gained
			m_HasFocus = true;
			LISTENER->OnWindowFocusGained(this);
			break;

		case WM_KILLFOCUS:	// Focus lost
			m_HasFocus = false;
			LISTENER->OnWindowFocusLost(this);
			break;

		case WM_SIZE:	// Window resize
			if (wParam == SIZE_MINIMIZED)		// Window minimized
			{
				m_IsShown = false;
				LISTENER->OnWindowMinimize(this);
				break;
			}
			else if (wParam == SIZE_RESTORED)	// Window restored
			{
				m_IsShown = true;
				LISTENER->OnWindowRestore(this);
			}
			else if (wParam == SIZE_MAXIMIZED)	// Window maximized
			{
				m_IsShown = true;
				LISTENER->OnWindowMaximize(this);
			}
			break;

			// Get the window size and notify the listener
			RECT wndRect;
			GetClientRect(hWnd, &wndRect);
			m_Width = wndRect.right - wndRect.left;
			m_Height = wndRect.bottom - wndRect.top;
			LISTENER->OnWindowResize(this, m_Width, m_Height);
			break;

		case WM_MOVE:	// Window moved
			m_XPos = (int)(short)LOWORD(lParam);
			m_YPos = (int)(short)HIWORD(lParam);
			LISTENER->OnWindowMove(this, m_XPos, m_YPos);
			break;

		/************* KEYBOARD MESSAGES *************/
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (!(lParam & 0x40000000) || m_Keyboard.IsAutorepeatEnabled())
			{
				m_Keyboard.OnKeyPress(static_cast<unsigned char>(wParam));
				LISTENER->OnKeyPress(this, static_cast<Key>(wParam));
			}				
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			m_Keyboard.OnKeyRelease(static_cast<unsigned char>(wParam));
			LISTENER->OnKeyRelease(this, static_cast<Key>(wParam));
			break;

		case WM_CHAR:
			m_Keyboard.OnChar(static_cast<char>(wParam));
			LISTENER->OnCharInput(this, static_cast<char>(wParam));
			break;
		/*********** END KEYBOARD MESSAGES ***********/

		default:
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
#pragma endregion

#pragma region Window Exceptions

	std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
	{
		char* pMsgBuf = nullptr;
		DWORD msgLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
		);

		if (msgLen == 0) return "Undefined Error Code";

		std::string errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}

	Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
		:
		Exception(line, file), m_Hr(hr)
	{
	}

	const char* Window::HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl
			<< GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
 	}

	const char* Window::HrException::GetType() const noexcept
	{
		return "Win32 Window Exception";
	}

	HRESULT Window::HrException::GetErrorCode() const noexcept
	{
		return m_Hr;
	}

	std::string Window::HrException::GetErrorDescription() const noexcept
	{
		return TranslateErrorCode(m_Hr);
	}

#pragma endregion
}
