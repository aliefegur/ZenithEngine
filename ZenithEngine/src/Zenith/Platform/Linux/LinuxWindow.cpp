#include "zenithpch.h"
#include "LinuxWindow.h"

namespace Zenith
{
	bool LinuxWindow::glfwInitialized = false;
	bool LinuxWindow::gladInitialized = false;

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

		// Create rendering context
		glfwMakeContextCurrent(m_Window);

		InitGLAD();

		// Enable V-Sync
		glfwSwapInterval(1);
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
