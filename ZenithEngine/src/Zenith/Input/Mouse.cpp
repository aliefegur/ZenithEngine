#include "zenithpch.h"
#include "Mouse.h"
#include <Windows.h>

namespace Zenith
{
	std::pair<int, int> Mouse::GetPos() const noexcept
	{
		return { m_X, m_Y };
	}

	int Mouse::GetPosX() const noexcept
	{
		return m_X;
	}

	int Mouse::GetPosY() const noexcept
	{
		return m_Y;
	}

	bool Mouse::IsInWindow() const noexcept
	{
		return m_IsInWindow;
	}

    bool Mouse::IsButtonPressed(MouseButton button) const noexcept
    {
        return m_ButtonStates[static_cast<size_t>(button)];
    }

    bool Mouse::IsButtonJustPressed(MouseButton button) const noexcept
    {
		size_t btn = static_cast<size_t>(button);
        return m_ButtonStates[btn] && !m_LastButtonStates[btn];
    }

    bool Mouse::IsButtonJustReleased(MouseButton button) const noexcept
    {
        size_t btn = static_cast<size_t>(button);
        return !m_ButtonStates[btn] && m_LastButtonStates[btn];
    }

    Mouse::Event Mouse::Read() noexcept
    {
        if (m_EventBuffer.size() > 0u)
        {
            Event e = m_EventBuffer.front();
            m_EventBuffer.pop();
            return e;
        }
        else
        {
            return Event();
        }
    }

    bool Mouse::IsEmpty() const noexcept
    {
        return m_EventBuffer.empty();
    }

    void Mouse::Flush() noexcept
    {
		m_LastButtonStates = m_ButtonStates;
        m_EventBuffer = std::queue<Event>();
    }

    void Mouse::OnMouseMove(int x, int y) noexcept
    {
        this->m_X = x;
        this->m_Y = y;

        m_EventBuffer.push(Event(Event::Type::Move, 0, *this));
        TrimBuffer();
    }

    void Mouse::OnMouseEnter() noexcept
    {
        m_IsInWindow = true;
        m_EventBuffer.push(Event(Event::Type::Enter, 0, *this));
        TrimBuffer();
    }

    void Mouse::OnMouseLeave() noexcept
    {
        m_IsInWindow = false;
        m_EventBuffer.push(Event(Event::Type::Leave, 0, *this));
        TrimBuffer();
    }

    void Mouse::OnButtonPressed(int x, int y, unsigned char code) noexcept
    {
		m_ButtonStates[code] = true;
        m_EventBuffer.push(Event(Event::Type::Press, code, *this));
		TrimBuffer();
    }

    void Mouse::OnButtonReleased(int x, int y, unsigned char code) noexcept
    {
        m_ButtonStates[code] = false;
		m_EventBuffer.push(Event(Event::Type::Release, code, *this));
		TrimBuffer();
    }

    void Mouse::OnWheelUp(int x, int y) noexcept
    {
        m_EventBuffer.push(Event(Event::Type::WheelUp, NULL, *this));
        TrimBuffer();
    }

    void Mouse::OnWheelDown(int x, int y) noexcept
    {
        m_EventBuffer.push(Event(Event::Type::WheelDown, NULL, *this));
        TrimBuffer();
    }

    void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
    {
        m_WheelDelta += delta;

        while (m_WheelDelta >= WHEEL_DELTA)
        {
            m_WheelDelta -= WHEEL_DELTA;
            OnWheelUp(x, y);
        }

        while (m_WheelDelta <= WHEEL_DELTA)
        {
            m_WheelDelta += WHEEL_DELTA;
            OnWheelDown(x, y);
        }
    }

    void Mouse::TrimBuffer() noexcept
    {
        while (m_EventBuffer.size() > m_BufferSize)
        {
            m_EventBuffer.pop();
        }
    }
}
