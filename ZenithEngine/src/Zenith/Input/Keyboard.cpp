#include "zenithpch.h"
#include "Keyboard.h"

namespace Zenith
{
	Keyboard::Event::Event() noexcept
		:
		m_Type(Type::Invalid), m_Code(0u)
	{
	}
	Keyboard::Event::Event(Type type, unsigned char code) noexcept
		:
		m_Type(type), m_Code(code)
	{
	}
	bool Keyboard::Event::IsPress() const noexcept
	{
		return m_Type == Type::Press;
	}
	bool Keyboard::Event::IsRelease() const noexcept
	{
		return m_Type == Type::Release;
	}
	bool Keyboard::Event::IsValid() const noexcept
	{
		return m_Type != Type::Invalid;
	}
	unsigned char Keyboard::Event::GetCode() const noexcept
	{
		return m_Code;
	}

	bool Keyboard::IsKeyPressed(Key key) const noexcept
	{
		const uint16_t keycode = static_cast<uint16_t>(key);
		return m_KeyStates[keycode];
	}
	bool Keyboard::IsKeyJustPressed(Key key) const noexcept
	{
		const uint16_t keycode = static_cast<uint16_t>(key);
		return m_KeyStates[keycode] && !m_LastStates[keycode];
	}
	Keyboard::Event Keyboard::ReadKey() noexcept
	{
		if (m_KeyBuffer.size() > 0)
		{
			Event e = m_KeyBuffer.front();
			m_KeyBuffer.pop();
			return e;
		}

		return Event();
	}
	bool Keyboard::IsEmpty() const noexcept
	{
		return m_KeyBuffer.empty();
	}
	void Keyboard::FlushKey() noexcept
	{
		m_LastStates = m_KeyStates;
		m_KeyBuffer = std::queue<Event>();
	}

	char Keyboard::ReadChar() noexcept
	{
		if (m_CharBuffer.size() > 0)
		{
			unsigned char charcode = m_CharBuffer.front();
			m_CharBuffer.pop();
			return charcode;
		}

		return 0;
	}
	bool Keyboard::IsCharEmpty() const noexcept
	{
		return m_CharBuffer.empty();
	}
	void Keyboard::FlushChar() noexcept
	{
		m_CharBuffer = std::queue<char>();
	}

	void Keyboard::Flush() noexcept
	{
		FlushKey();
		FlushChar();
	}

	void Keyboard::EnableAutorepeat() noexcept
	{
		m_AutorepeatEnabled = true;
	}
	void Keyboard::DisableAutorepeat() noexcept
	{
		m_AutorepeatEnabled = false;
	}
	bool Keyboard::IsAutorepeatEnabled() const noexcept
	{
		return m_AutorepeatEnabled;
	}

	void Keyboard::OnKeyPress(Key key) noexcept
	{
		const uint16_t keycode = static_cast<uint16_t>(key);
		m_KeyStates[keycode] = true;
		m_KeyBuffer.push(Event(Event::Type::Press, keycode));
		TrimBuffer(m_KeyBuffer);
	}
	void Keyboard::OnKeyRelease(Key key) noexcept
	{
		const uint16_t keycode = static_cast<uint16_t>(key);
		m_KeyStates[keycode] = false;
		m_KeyBuffer.push(Event(Event::Type::Release, keycode));
		TrimBuffer(m_KeyBuffer);
	}
	void Keyboard::OnChar(char character) noexcept
	{
		m_CharBuffer.push(character);
		TrimBuffer(m_CharBuffer);
	}

	void Keyboard::ClearState() noexcept
	{
		m_KeyStates.reset();
		m_LastStates.reset();
	}
}
