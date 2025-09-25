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
		static void window_close_callback(GLFWwindow* window);
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);
		static void window_maximize_callback(GLFWwindow* window, int maximized);
		static void window_iconify_callback(GLFWwindow* window, int iconified);
		static void window_focus_callback(GLFWwindow* window, int focused);
		static void window_refresh_callback(GLFWwindow* window);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void char_callback(GLFWwindow* window, unsigned int codepoint);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static void cursor_enter_callback(GLFWwindow* window, int entered);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	private:
        static void InitGLFW();
        static void TerminateGLFW();
        static bool glfwInitialized;
	};
}

#endif // ZENITH_PLATFORM_LINUX
