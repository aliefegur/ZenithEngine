#pragma once

#include "Key.h"

namespace Zenith
{
	class EventListener
	{
		friend class Window;
	public:
		EventListener() = default;
		EventListener(const EventListener&) = delete;
		EventListener& operator=(const EventListener&) = delete;
		virtual ~EventListener() = default;

	protected:
		virtual void OnWindowResize(Window* window, int width, int height) {}
		virtual void OnWindowFocusGained(Window* window) {}
		virtual void OnWindowFocusLost(Window* window) {}
		virtual void OnKeyPress(Window* window, Key key) {}
		virtual void OnKeyRelease(Window* window, Key key) {}
		virtual void OnCharInput(Window* window, char character) {}
		virtual void OnMouseMove(Window* window, int x, int y) {}
		virtual void OnMouseButtonPress(Window* window, int button, int x, int y) {}
		virtual void OnMouseButtonRelease(Window* window, int button, int x, int y) {}
		virtual void OnMouseScroll(Window* window, int deltaX, int deltaY) {}
		virtual void OnMouseEnter(Window* window) {}
		virtual void OnMouseLeave(Window* window) {}
		virtual void OnWindowClose(Window* window) {}
		virtual void OnWindowMinimize(Window* window) {}
		virtual void OnWindowRestore(Window* window) {}
		virtual void OnWindowMaximize(Window* window) {}
		virtual void OnWindowMove(Window* window, int x, int y) {}
	};
}
