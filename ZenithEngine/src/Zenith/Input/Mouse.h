#pragma once

#include <queue>
#include <bitset>
#include <utility>
#include "Key.h"

namespace Zenith
{
	class Mouse
	{
		friend class Window;
	public:
		class Event
		{
		public:
			enum class Type 
			{ 
				Press,
				Release,
				WheelUp, 
				WheelDown, 
				Move, 
				Enter, 
				Leave, 
				Invalid
			};
		public:
			Event() noexcept : m_Type(Type::Invalid), m_Code(0), m_X(0), m_Y(0) { }
			Event(Type type, unsigned char code, Mouse& parent) : m_Type(type), m_Code(code), m_X(parent.m_X), m_Y(parent.m_Y) {}
			bool IsValid() const noexcept { return m_Type != Type::Invalid; }
			Type GetType() const noexcept { return m_Type; }
			std::pair<int, int> GetPos() const noexcept { return { m_X, m_Y }; }
			int GetPosX() const noexcept { return m_X; }
			int GetPosY() const noexcept { return m_Y; }
			int GetCode() const noexcept { return m_Code; }
		private:
			Type m_Type;
			unsigned char m_Code;
			int m_X, m_Y;
		};
	
	public:
		Mouse() = default;
		Mouse(const Mouse&) = delete;
		Mouse& operator=(const Mouse&) = delete;
		std::pair<int, int> GetPos() const noexcept;
		int GetPosX() const noexcept;
		int GetPosY() const noexcept;
		bool IsInWindow() const noexcept;
		bool IsButtonPressed(MouseButton button) const noexcept;
		bool IsButtonJustPressed(MouseButton button) const noexcept;
		bool IsButtonJustReleased(MouseButton button) const noexcept;
		Event Read() noexcept;
		bool IsEmpty() const noexcept;
		void Flush() noexcept;

	private:
		void OnMouseMove(int x, int y) noexcept;
		void OnMouseEnter() noexcept;
		void OnMouseLeave() noexcept;
		void OnButtonPressed(int x, int y, unsigned char code) noexcept;
		void OnButtonReleased(int x, int y, unsigned char code) noexcept;
		void OnWheelUp(int x, int y) noexcept;
		void OnWheelDown(int x, int y) noexcept;
		void OnWheelDelta(int x, int y, int delta) noexcept;
		void TrimBuffer() noexcept;

	private:
		static constexpr unsigned int m_BufferSize = 16u;
		int		m_X = 0,
				m_Y = 0,
				m_WheelDelta = 0;
		bool	m_IsInWindow = false;
		
		std::bitset<4> m_ButtonStates;
		std::bitset<4> m_LastButtonStates;
		std::queue<Event> m_EventBuffer;
	};
}
