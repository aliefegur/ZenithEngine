#pragma once

namespace Zenith
{
	enum class Key
	{
#if ZENITH_PLATFORM_WINDOWS
		// Letters
		A = 0x41, B = 0x42, C = 0x43, D = 0x44, E = 0x45, F = 0x46, G = 0x47,
		H = 0x48, I = 0x49, J = 0x4a, K = 0x4b, L = 0x4c, M = 0x4d, N = 0x4e,
		O = 0x4f, P = 0x50, Q = 0x51, R = 0x52, S = 0x53, T = 0x54, U = 0x55,
		V = 0x56, W = 0x57, X = 0x58, Y = 0x59, Z = 0x5a,

		// Number row
		Zero = 0x30, One = 0x31, Two = 0x32, Three = 0x33, Four = 0x34,
		Five = 0x35, Six = 0x36, Seven = 0x37, Eight = 0x38, Nine = 0x39,

		// Numpad
		Num0 = 0x60, Num1 = 0x61, Num2 = 0x62, Num3 = 0x63, Num4 = 0x64,
		Num5 = 0x65, Num6 = 0x66, Num7 = 0x67, Num8 = 0x68, Num9 = 0x69,

		Escape = 0x1b,
		F1 = 0x70, F2 = 0x71, F3 = 0x72, F4 = 0x73, F5 = 0x74, F6 = 0x75,
		F7 = 0x76, F8 = 0x77, F9 = 0x78, F10 = 0x79, F11 = 0x7a, F12 = 0x7b,

		Delete = 0x2e,
		Minus = 0xbd,
		Backspace = 0x8,
		Enter = 0xd,
		Shift = 0x10,
		Ctrl = 0x11,
		Alt = 0x12,
		Space = 0x20,

		Left = 0x25, Right = 0x27, Up = 0x26, Down = 0x28,
		Home = 0x24, End = 0x23, PageUp = 0x21, PageDown = 0x22,

		NumLock = 0x90,
		NumSlash = 0x6f,
		NumAsterix = 0x6a,
		NumMinus = 0x6d,
		NumPlus = 0x6b,
		NumComma = 0x6e,

		Dot = 0xbe,
		Comma = 0xbc,
		Tab = 0x9,
		CapsLock = 0x14,
		Quotes = 0xc0,
		LessThan = 0xe2

#elif ZENITH_PLATFORM_LINUX
		A = 65, B = 66, C = 67, D = 68, E = 69,
		F = 70, G = 71, H = 72, I = 73, J = 74,
		K = 75, L = 76, M = 77, N = 78, O = 79,
		P = 80, Q = 81, R = 82, S = 83, T = 84,
		U = 85, V = 86, W = 87, X = 88, Y = 89,
		Z = 90,

		Zero = 48, One = 49, Two = 50, Three = 51,
		Four = 52, Five = 53, Six = 54, Seven = 55,
		Eight = 56, Nine = 57,

		Num0 = 320, Num1 = 321, Num2 = 322, Num3 = 323,
		Num4 = 324, Num5 = 325, Num6 = 326, Num7 = 327,
		Num8 = 328, Num9 = 329,

		Escape = 256,
		F1 = 290, F2 = 291, F3 = 292, F4 = 293,
		F5 = 294, F6 = 295, F7 = 296, F8 = 297,
		F9 = 298, F10 = 299, F11 = 300, F12 = 301,

		Delete = 261,
		Minus = 269,
		Backspace = 259,
		Enter = 257,
		Shift = 340,
		Ctrl = 341,
		Alt = 342,
		Space = 32,

		Left = 263,
		Right = 262,
		Up = 265,
		Down = 264,
		Home = 268,
		End = 269,
		PageUp = 266,
		PageDown = 267,

		NumLock = 282,
		NumSlash = 320,
		NumAsterix = 321,
		NumMinus = 322,
		NumPlus = 323,
		NumComma = 324,

		Dot = 46,
		Comma = 44,
		Tab = 258,
		CapsLock = 280,
		Quotes = 39,
		LessThan = 96
#endif
	};
	
	enum class MouseButton
	{
#if GLFW_PLATFORM_WINDOWS
		Left = 0x01,
		Middle = 0x02,
		Right = 0x03,
#elif GLFW_PLATFORM_LINUX
		Left = 0,
		Right = 1,
		Middle = 2
#endif
	};
}
