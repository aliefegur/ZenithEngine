#pragma once

#include "Zenith/Renderer/Graphics.h"
#include "Zenith/Utils/ZenithException.h"

#include <Windows.h>

namespace Zenith
{
	class Window;

	class OpenGLGraphics : public Graphics
	{
	public:
		class FramebufferSwapError : public ZenithException
		{
			using ZenithException::ZenithException;
		public:
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
		};

	public:
		OpenGLGraphics(Window& targetWindow);
		~OpenGLGraphics();

		void EndFrame() override;
		void ClearBuffer(float red, float green, float blue, float alpha) noexcept override;
		void DrawIndexed(unsigned int count) override;

	private:
		HGLRC	m_Context;
		HDC		m_Device;
	};
}
