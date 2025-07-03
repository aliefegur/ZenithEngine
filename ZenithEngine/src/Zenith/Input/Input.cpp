#include "zenithpch.h"
#include "Input.h"

namespace Zenith
{
	static Window* s_EventWindow = nullptr;

	bool Input::IsKeyPressed(Key key) noexcept
	{
		return s_EventWindow->m_Keyboard.IsKeyPressed(static_cast<unsigned char>(key));
	}

	bool Input::IsKeyPressed(unsigned char key) noexcept
	{
		return s_EventWindow->m_Keyboard.IsKeyPressed(key);
	}

	bool Input::IsKeyJustPressed(Key key) noexcept
	{
		return s_EventWindow->m_Keyboard.IsKeyJustPressed(static_cast<unsigned char>(key));
	}

	bool Input::IsKeyJustPressed(unsigned char key) noexcept
	{
		return s_EventWindow->m_Keyboard.IsKeyJustPressed(key);
	}
	
	std::vector<unsigned char> Input::GetPressingKeys() noexcept
	{
		std::vector<unsigned char> pressedKeys;

		for (unsigned short i = 0; i < s_EventWindow->m_Keyboard.s_KeyCount; i++)
		{
			if (s_EventWindow->m_Keyboard.IsKeyPressed(static_cast<unsigned char>(i)))
			{
				pressedKeys.push_back(static_cast<unsigned char>(i));
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
