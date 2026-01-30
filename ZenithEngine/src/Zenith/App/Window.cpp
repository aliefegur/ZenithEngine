#include "zenithpch.h"

#include "Window.h"
#include "Zenith/Renderer/Graphics.h"
#include "Zenith/Platform/OpenGL/OpenGLGraphics.h"
#if ZENITH_PLATFORM_WINDOWS
#include "Zenith/Platform/Windows/Win32Window.h"
#include "Zenith/Platform/DirectX/D3D11Graphics.h"
#elif ZENITH_PLATFORM_LINUX
#include "Zenith/Platform/Linux/LinuxWindow.h"
#endif

#include <imgui.h>

namespace Zenith
{
	Window* Window::Create(int width, int height, const std::string& title, bool fullScreen)
	{
#if ZENITH_PLATFORM_WINDOWS
		return new Win32Window(width, height, title, fullScreen);
#elif ZENITH_PLATFORM_LINUX
		return new LinuxWindow(width, height, title, fullScreen);
#endif
	}

	Window::Window(int width, int height, const std::string& title, bool fullScreen)
		:
		m_Width(width), m_Height(height), m_Title(title), m_IsFullscreen(fullScreen), m_Graphics(nullptr), m_HasFocus(false), m_IsShown(false), m_EventListener(nullptr), m_XPos(0), m_YPos(0)
	{
	}

	Window::~Window()
	{
	}

	void Window::BindEventListener(EventListener* eventListener) noexcept
	{
		m_EventListener = eventListener;
	}

	void Window::SetWidth(int width)
	{
		m_Width = width;
	}
	void Window::SetHeight(int height)
	{
		m_Height = height;
	}
	void Window::SetTitle(const std::string& title)
	{
		m_Title = title;		
	}
	void Window::SetFullscreenState(bool fullScreen)
	{
		m_IsFullscreen = fullScreen;
	}
	void Window::Show()
	{
		m_IsShown = true;
	}
	void Window::Hide() 
	{
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
		return m_Title;
	}
	Graphics* Window::GetGfx() const noexcept
	{
		return m_Graphics;
	}

	void Window::ProcessEvents()
	{
		m_Keyboard.Flush();
		m_Mouse.Flush();
	}

	void Window::CreateGraphicsContext(Graphics::API targetApi)
	{
		switch (targetApi)
		{
		case Zenith::Graphics::API::D3D11: 
#if ZENITH_PLATFORM_WINDOWS
			m_Graphics = new D3D11Graphics(*this);
#else
			// TODO: Throw error
			m_Graphics = nullptr;
#endif
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
}
