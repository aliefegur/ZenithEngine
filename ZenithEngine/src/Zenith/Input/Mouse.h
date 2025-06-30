#pragma once

#include <queue>

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
				LPress, 
				LRelease, 
				RPress, 
				RRelease, 
				WheelUp, 
				WheelDown, 
				Move, 
				Enter, 
				Leave, 
				Invalid
			};
		public:
			Event() noexcept : m_LeftIsPressed(false), m_RightIsPressed(false), m_X(0), m_Y(0), m_Type(Type::Invalid) { }
			Event(Type type, Mouse& parent) : m_Type(type), m_LeftIsPressed(parent.m_LeftIsPressed), m_RightIsPressed(parent.m_RightIsPressed), m_X(parent.m_X), m_Y(parent.m_Y) {}
			bool IsValid() const noexcept { return m_Type != Type::Invalid; }
			Type GetType() const noexcept { return m_Type; }
			std::pair<int, int> GetPos() const noexcept { return { m_X, m_Y }; }
			int GetPosX() const noexcept { return m_X; }
			int GetPosY() const noexcept { return m_Y; }
			bool LeftIsPressed() const noexcept { return m_LeftIsPressed; }
			bool RightIsPressed() const noexcept { return m_RightIsPressed; }
		private:
			Type m_Type;
			bool m_LeftIsPressed, m_RightIsPressed;
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
		bool LeftIsPressed() const noexcept;
		bool RightIsPressed() const noexcept;
		Event Read() noexcept;
		bool IsEmpty() const noexcept;
		void Flush() noexcept;

	private:
		void OnMouseMove(int x, int y) noexcept;
		void OnMouseEnter() noexcept;
		void OnMouseLeave() noexcept;
		void OnLeftPressed(int x, int y) noexcept;
		void OnLeftReleased(int x, int y) noexcept;
		void OnRightPressed(int x, int y) noexcept;
		void OnRightReleased(int x, int y) noexcept;
		void OnWheelUp(int x, int y) noexcept;
		void OnWheelDown(int x, int y) noexcept;
		void OnWheelDelta(int x, int y, int delta) noexcept;
		void TrimBuffer() noexcept;

	private:
		static constexpr unsigned int m_BufferSize = 16u;
		int		m_X = 0,
				m_Y = 0,
				m_WheelDelta = 0;
		bool	m_LeftIsPressed = false,
				m_RightIsPressed = false,
				m_IsInWindow = false;
		std::queue<Event> m_EventBuffer;
	};
}
