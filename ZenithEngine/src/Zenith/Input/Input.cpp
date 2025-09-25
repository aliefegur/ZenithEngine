#include "zenithpch.h"
#include "Input.h"

namespace Zenith
{
	static Window* s_EventWindow = nullptr;

	bool Input::IsKeyPressed(Key key) noexcept
	{
		return s_EventWindow->m_Keyboard.IsKeyPressed(key);
	}

	bool Input::IsKeyPressed(uint16_t key) noexcept
	{
		return s_EventWindow->m_Keyboard.IsKeyPressed(static_cast<Key>(key));
	}

	bool Input::IsKeyJustPressed(Key key) noexcept
	{
		return s_EventWindow->m_Keyboard.IsKeyJustPressed(key);
	}

	bool Input::IsKeyJustPressed(uint16_t key) noexcept
	{
		return s_EventWindow->m_Keyboard.IsKeyJustPressed(static_cast<Key>(key));
	}
	
	std::vector<uint16_t> Input::GetPressingKeys() noexcept
	{
		std::vector<uint16_t> pressedKeys;

		for (uint16_t i = 0; i < s_EventWindow->m_Keyboard.s_KeyCount; i++)
		{
			if (s_EventWindow->m_Keyboard.IsKeyPressed(static_cast<Key>(i)))
			{
				pressedKeys.push_back(i);
			}
		}

		return pressedKeys;
	}

	void Input::SetupEventWindow(Window* eventWnd) noexcept
	{
		s_EventWindow = eventWnd;
	}

	std::pair<int, int> Input::GetMousePosition() noexcept
	{
		return s_EventWindow->m_Mouse.GetPos();
	}

	int Input::GetMousePosX() noexcept
	{
		return s_EventWindow->m_Mouse.GetPosX();
	}

	int Input::GetMousePosY() noexcept
	{
		return s_EventWindow->m_Mouse.GetPosY();
	}

	bool Input::IsMouseInWindow() noexcept
	{
		return s_EventWindow->m_Mouse.IsInWindow();
	}

	bool Input::IsMouseButtonPressed(MouseButton button) noexcept
	{
		return s_EventWindow->m_Mouse.IsButtonPressed(button);
	}

	bool Input::IsMouseButtonJustPressed(MouseButton button) noexcept
	{
		return s_EventWindow->m_Mouse.IsButtonJustPressed(button);
	}

	bool Input::IsMouseButtonJustReleased(MouseButton button) noexcept
	{
		return s_EventWindow->m_Mouse.IsButtonJustReleased(button);
	}
}
