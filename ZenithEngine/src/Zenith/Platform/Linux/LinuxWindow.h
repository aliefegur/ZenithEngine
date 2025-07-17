#if ZENITH_PLATFORM_LINUX

#pragma once

#include "Zenith/App/Window.h"

struct GLFWwindow;

namespace Zenith
{
	class LinuxWindow : public Window
	{
	public:
		LinuxWindow(int width, int height, const std::string& title, bool fullScreen);
		~LinuxWindow();

		void SetWidth(int width) override;
		void SetHeight(int height) override;
		void SetTitle(const std::string& title) override;
		void SetFullscreenState(bool fullScreen) override;
		void Show() override;
		void Hide() override;

		void ProcessEvents() override;

		GLFWwindow* GetWindowHandle() const noexcept;

	private:
		GLFWwindow* m_Window;

	private:
        static void InitGLFW();
        static void TerminateGLFW();
        static bool glfwInitialized;
	};
}

#endif // ZENITH_PLATFORM_LINUX
