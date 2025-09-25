#pragma once

#include "Zenith/Utils/ZenithException.h"
#include "Zenith/Renderer/Graphics.h"
#include "Zenith/Input/Keyboard.h"
#include "Zenith/Input/Mouse.h"
#include "Zenith/Input/EventListener.h"

namespace Zenith
{
	class Window
	{		
		friend class Input;
	public:
		Window(int width, int height, const std::string& title, bool fullScreen);
		virtual ~Window();

		void BindEventListener(EventListener* eventListener) noexcept;

		virtual void SetWidth(int width);
		virtual void SetHeight(int height);
		virtual void SetTitle(const std::string& title);
		virtual void SetFullscreenState(bool fullScreen);
		virtual void Show();
		virtual void Hide();

		int GetWidth() const noexcept;
		int GetHeight() const noexcept; 
		std::string GetTitle() const noexcept;
		Graphics* GetGfx() const noexcept;

		virtual void ProcessEvents();
		void CreateGraphicsContext(Graphics::API targetApi);

		/*
	protected:
		void OnWindowResize(Window* window, int width, int height);
		void OnWindowFocusGained(Window* window);
		void OnWindowFocusLost(Window* window);
		void OnKeyPress(Window* window, Key key);
		void OnKeyRelease(Window* window, Key key);
		void OnCharInput(Window* window, char character);
		void OnMouseMove(Window* window, int x, int y);
		void OnMouseButtonPress(Window* window, MouseButton button, int x, int y);
		void OnMouseButtonRelease(Window* window, MouseButton button, int x, int y);
		void OnMouseScroll(Window* window, int deltaX, int deltaY);
		void OnMouseEnter(Window* window);
		void OnMouseLeave(Window* window);
		void OnWindowClose(Window* window);
		void OnWindowMinimize(Window* window);
		void OnWindowRestore(Window* window);
		void OnWindowMaximize(Window* window);
		void OnWindowMove(Window* window, int x, int y);
		*/

	protected:
		int m_XPos, m_YPos;
		int m_Width, m_Height;
		std::string m_Title;
		bool	m_IsFullscreen, 
				m_IsShown,
				m_HasFocus;
		Graphics* m_Graphics;
		Keyboard m_Keyboard;
		Mouse m_Mouse;
		EventListener* m_EventListener;
	};
}
