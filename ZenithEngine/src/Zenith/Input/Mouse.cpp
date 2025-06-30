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

    bool Mouse::LeftIsPressed() const noexcept
    {
        return m_LeftIsPressed;
    }

    bool Mouse::RightIsPressed() const noexcept
    {
        return m_RightIsPressed;
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
        m_EventBuffer = std::queue<Event>();
    }

    void Mouse::OnMouseMove(int x, int y) noexcept
    {
        this->m_X = x;
        this->m_Y = y;

        m_EventBuffer.push(Event(Event::Type::Move, *this));
        TrimBuffer();
    }

    void Mouse::OnMouseEnter() noexcept
    {
        m_IsInWindow = true;
        m_EventBuffer.push(Event(Event::Type::Enter, *this));
        TrimBuffer();
    }

    void Mouse::OnMouseLeave() noexcept
    {
        m_IsInWindow = false;
        m_EventBuffer.push(Event(Event::Type::Leave, *this));
        TrimBuffer();
    }

    void Mouse::OnLeftPressed(int x, int y) noexcept
    {
        m_LeftIsPressed = true;

        m_EventBuffer.push(Event(Event::Type::LPress, *this));
        TrimBuffer();
    }

    void Mouse::OnLeftReleased(int x, int y) noexcept
    {
        m_LeftIsPressed = false;

        m_EventBuffer.push(Event(Event::Type::LRelease, *this));
        TrimBuffer();
    }

    void Mouse::OnRightPressed(int x, int y) noexcept
    {
        m_RightIsPressed = true;

        m_EventBuffer.push(Event(Event::Type::RPress, *this));
        TrimBuffer();
    }

    void Mouse::OnRightReleased(int x, int y) noexcept
    {
        m_RightIsPressed = false;

        m_EventBuffer.push(Event(Event::Type::RRelease, *this));
        TrimBuffer();
    }

    void Mouse::OnWheelUp(int x, int y) noexcept
    {
        m_EventBuffer.push(Event(Event::Type::WheelUp, *this));
        TrimBuffer();
    }

    void Mouse::OnWheelDown(int x, int y) noexcept
    {
        m_EventBuffer.push(Event(Event::Type::WheelDown, *this));
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
