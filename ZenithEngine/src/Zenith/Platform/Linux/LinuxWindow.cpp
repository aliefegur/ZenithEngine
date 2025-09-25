#include "zenithpch.h"
#include "LinuxWindow.h"
#include "Zenith/App/AppManager.h"

#define LISTENER if (wnd->m_EventListener) wnd->m_EventListener

namespace Zenith
{
	bool LinuxWindow::glfwInitialized = false;

	LinuxWindow::LinuxWindow(int width, int height, const std::string &title, bool fullScreen)
		:
		Window(width, height, title, fullScreen),
		m_Window(nullptr)
	{
		InitGLFW();

		// Minimum OpenGL version: 4.6
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Forward compatibility for macOS
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // Allow resizing the window

		// Create window
		m_Window = glfwCreateWindow(width, height, title.c_str(), fullScreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if (!m_Window)
		{
			glfwTerminate();
			// TODO: Throw an error
			throw "Failed to create GLFW window!";
		}

		// Set user pointer for GLFW callbacks
		glfwSetWindowUserPointer(m_Window, this);

		// Set event callbacks
		glfwSetWindowCloseCallback(m_Window, window_close_callback);
		glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
		glfwSetWindowPosCallback(m_Window, window_pos_callback);
		glfwSetWindowMaximizeCallback(m_Window, window_maximize_callback);
		glfwSetWindowIconifyCallback(m_Window, window_iconify_callback);
		glfwSetWindowFocusCallback(m_Window, window_focus_callback);
		glfwSetWindowRefreshCallback(m_Window, window_refresh_callback);
		glfwSetKeyCallback(m_Window, key_callback);
		glfwSetCharCallback(m_Window, char_callback);
		glfwSetCursorPosCallback(m_Window, cursor_position_callback);
		glfwSetCursorEnterCallback(m_Window, cursor_enter_callback);
		glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
		glfwSetScrollCallback(m_Window, scroll_callback);
	}
	
	LinuxWindow::~LinuxWindow()
	{
		glfwDestroyWindow(m_Window);
	}

	void LinuxWindow::SetWidth(int width)
	{
		// TODO: Implement this
	}

	void LinuxWindow::SetHeight(int height)
	{
		// TODO: Implement this
	}

	void LinuxWindow::SetTitle(const std::string &title)
	{
		// TODO: Implement this
	}

	void LinuxWindow::SetFullscreenState(bool fullScreen)
	{
		// TODO: Implement this
	}

	void LinuxWindow::Show()
	{
		glfwShowWindow(m_Window);
		m_IsShown = true;
	}

	void LinuxWindow::Hide()
	{
		glfwHideWindow(m_Window);
		m_IsShown = false;
	}

	void LinuxWindow::ProcessEvents()
	{
		glfwPollEvents();
	}

	GLFWwindow* LinuxWindow::GetWindowHandle() const noexcept
	{
		return m_Window;
	}

	void LinuxWindow::window_close_callback(GLFWwindow *window)
	{
		LinuxWindow* wnd = reinterpret_cast<LinuxWindow*>(glfwGetWindowUserPointer(window));
		wnd->m_IsShown = false;
		LISTENER->OnWindowClose(wnd);
		AppManager::QuitApplication(0);
	}

	void LinuxWindow::framebuffer_size_callback(GLFWwindow *window, int width, int height)
	{
		LinuxWindow* wnd = reinterpret_cast<LinuxWindow*>(glfwGetWindowUserPointer(window));
		LISTENER->OnWindowResize(wnd, width, height);
	}

	void LinuxWindow::window_pos_callback(GLFWwindow *window, int xpos, int ypos)
	{
		LinuxWindow* wnd = reinterpret_cast<LinuxWindow*>(glfwGetWindowUserPointer(window));
		LISTENER->OnWindowMove(wnd, xpos, ypos);
	}

	void LinuxWindow::window_maximize_callback(GLFWwindow *window, int maximized)
	{
		LinuxWindow* wnd = reinterpret_cast<LinuxWindow*>(glfwGetWindowUserPointer(window));
		if (maximized == GLFW_TRUE) 
		{
			wnd->m_IsShown = true;
			LISTENER->OnWindowMaximize(wnd);
		}
		else 
		{
			wnd->m_IsShown = true;
			LISTENER->OnWindowRestore(wnd);
		}
	}

	void LinuxWindow::window_iconify_callback(GLFWwindow* window, int iconified)
	{
		LinuxWindow* wnd = reinterpret_cast<LinuxWindow*>(glfwGetWindowUserPointer(window));
		if (iconified == GLFW_TRUE)
		{
			wnd->m_IsShown = false;
			LISTENER->OnWindowMinimize(wnd);
		}
		else
		{
			wnd->m_IsShown = true;
			LISTENER->OnWindowRestore(wnd);
		}
	}

	void LinuxWindow::window_focus_callback(GLFWwindow *window, int focused)
	{
		LinuxWindow* wnd = reinterpret_cast<LinuxWindow*>(glfwGetWindowUserPointer(window));
		wnd->m_HasFocus = focused == GLFW_TRUE;
		if (focused == GLFW_TRUE) LISTENER->OnWindowFocusGained(wnd);
		else LISTENER->OnWindowFocusLost(wnd);
	}

	void LinuxWindow::window_refresh_callback(GLFWwindow *window)
	{
		// TODO: Think about that callback, is it usable?
		// LinuxWindow* wnd = reinterpret_cast<LinuxWindow*>(glfwGetWindowUserPointer(window));
	}

	void LinuxWindow::key_callback(GLFWwindow *window, int keycode, int scancode, int action, int mods)
	{
		LinuxWindow* wnd = reinterpret_cast<LinuxWindow*>(glfwGetWindowUserPointer(window));
		const Key key = static_cast<Key>(keycode);
		Keyboard& kbd = wnd->m_Keyboard;

		switch (action)
		{
		case GLFW_PRESS:
			kbd.OnKeyPress(key);
			LISTENER->OnKeyPress(wnd, key);
			break;
		
		case GLFW_RELEASE:
			kbd.OnKeyRelease(key);
			LISTENER->OnKeyRelease(wnd, key);

		case GLFW_REPEAT:
			if (kbd.IsAutorepeatEnabled())
			{
				kbd.OnKeyPress(key);
				LISTENER->OnKeyPress(wnd, key);
			}

		default:
			break;
		}
	}

	void LinuxWindow::char_callback(GLFWwindow *window, unsigned int codepoint)
	{
		LinuxWindow* wnd = reinterpret_cast<LinuxWindow*>(glfwGetWindowUserPointer(window));
		const char chr = static_cast<char>(codepoint);
		wnd->m_Keyboard.OnChar(chr);
		LISTENER->OnCharInput(wnd, chr);
	}

	void LinuxWindow::cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
	{
		LinuxWindow* wnd = reinterpret_cast<LinuxWindow*>(glfwGetWindowUserPointer(window));
		wnd->m_Mouse.OnMouseMove(xpos, ypos);
		LISTENER->OnMouseMove(wnd, xpos, ypos);
	}

	void LinuxWindow::cursor_enter_callback(GLFWwindow *window, int entered)
	{
		LinuxWindow* wnd = reinterpret_cast<LinuxWindow*>(glfwGetWindowUserPointer(window));
		if (entered == GLFW_TRUE)
		{
			wnd->m_Mouse.OnMouseEnter();
			LISTENER->OnMouseEnter(wnd);
		}
		else
		{
			wnd->m_Mouse.OnMouseLeave();
			LISTENER->OnMouseLeave(wnd);
		}
	}

	void LinuxWindow::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
	{
		LinuxWindow* wnd = reinterpret_cast<LinuxWindow*>(glfwGetWindowUserPointer(window));
		
		int x, y;
		glfwGetWindowPos(window, &x, &y);

		switch (action)
		{
		case GLFW_PRESS:
			wnd->m_Mouse.OnButtonPressed(x, y, static_cast<unsigned char>(button));
			LISTENER->OnMouseButtonPress(wnd, static_cast<MouseButton>(button), x, y);
			break;
		case GLFW_RELEASE:
			wnd->m_Mouse.OnButtonReleased(x, y, static_cast<unsigned char>(button));
			LISTENER->OnMouseButtonRelease(wnd, static_cast<MouseButton>(button), x, y);
			break;
		
		default:
			break;
		}
	}

	void LinuxWindow::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
	{
		LinuxWindow* wnd = reinterpret_cast<LinuxWindow*>(glfwGetWindowUserPointer(window));

		int x, y;
		glfwGetWindowPos(window, &x, &y);

		wnd->m_Mouse.OnWheelDelta(x, y, static_cast<int>(yoffset));
		LISTENER->OnMouseScroll(wnd, static_cast<int>(xoffset), static_cast<int>(yoffset));
	}

	void LinuxWindow::InitGLFW()
	{
		if (!glfwInitialized)
		{
			if (!glfwInit())
			{
				// TODO: Throw an error
				throw "Failed to initialize GLFW!";
			}

			glfwInitialized = true;
		}
	}
	
	void LinuxWindow::TerminateGLFW()
	{
		if (glfwInitialized)
		{
			glfwTerminate();
			glfwInitialized = false;
		}
	}
}
