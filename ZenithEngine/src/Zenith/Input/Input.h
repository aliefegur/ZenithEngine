#pragma once

#include "Key.h"
#include "Zenith/App/Window.h"

#include <vector>
#include <utility>

namespace Zenith
{
	class Input
	{
		friend class ZenithApp;
	public:
		static void SetupEventWindow(Window* eventWnd) noexcept;

		// Keyboard input methods
		static bool IsKeyPressed(Key key) noexcept;
		static bool IsKeyPressed(unsigned char key) noexcept;
		static bool IsKeyJustPressed(Key key) noexcept;
		static bool IsKeyJustPressed(unsigned char key) noexcept;
		static std::vector<unsigned char> GetPressingKeys() noexcept;

		// Mouse input methods
		static std::pair<int, int> GetMousePosition() noexcept;
		static int GetMousePosX() noexcept;
		static int GetMousePosY() noexcept;
		static bool IsMouseInWindow() noexcept;
		static bool IsMouseButtonPressed(MouseButton button) noexcept;
		static bool IsMouseButtonJustPressed(MouseButton button) noexcept;
		static bool IsMouseButtonJustReleased(MouseButton button) noexcept;
	};
}
