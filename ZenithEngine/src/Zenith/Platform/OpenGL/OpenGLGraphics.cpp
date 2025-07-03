#include "zenithpch.h"
#include "OpenGLGraphics.h"
#include "Zenith/App/Window.h"

#define INIT_ERROR(_desc_) InitializationError(__LINE__, __FILE__, _desc_)
#define FBSWPERR() FramebufferSwapError(__LINE__, __FILE__)

namespace Zenith
{
#pragma region OpenGL Graphics

	extern int MAX_TEXTURES = 0;

	OpenGLGraphics::OpenGLGraphics(Window& targetWindow)
		:
		Graphics(targetWindow)
	{
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
			PFD_TYPE_RGBA,			// The kind of framebuffer. RGBA or palette.
			32,						// Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,						// Number of bits for the depthbuffer
			8,						// Number of bits for the stencilbuffer
			0,						// Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		m_Device = GetDC(targetWindow.GetHWND());

		int letWindowChoosePixelFormat = ChoosePixelFormat(m_Device, &pfd);
		SetPixelFormat(m_Device, letWindowChoosePixelFormat, &pfd);

		m_Context = wglCreateContext(m_Device);
		wglMakeCurrent(m_Device, m_Context);

		// Load OpenGL
		if (!gladLoadGL())
		{
			throw INIT_ERROR("Failed to load OpenGL functions!");
		}

		// Load GLAD WGL
		if (!gladLoadWGL(m_Device))
		{
			throw INIT_ERROR("Failed to load WGL extensions!");
		}

		// Initial viewport
		glViewport(0, 0, targetWindow.GetWidth(), targetWindow.GetHeight());

		// Enable V-Sync
		wglSwapIntervalEXT(1);

		// Enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Get graphical properties
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MAX_TEXTURES);

		std::cout << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;

		m_CurrentAPI = Graphics::API::OpenGL;

		InitializeImGui();
	}

	OpenGLGraphics::~OpenGLGraphics()
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(m_Context);
		ReleaseDC(m_TargetWindow.GetHWND(), m_Device);
	}

	void OpenGLGraphics::EndFrame()
	{
		Graphics::EndFrame();

		if (SwapBuffers(m_Device) == FALSE)
		{
			throw FBSWPERR();
		}
	}

	void OpenGLGraphics::ClearBuffer(float red, float green, float blue, float alpha) noexcept
	{
		Graphics::ClearBuffer(red, green, blue, alpha);

		glClearColor(red, green, blue, alpha);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLGraphics::DrawIndexed(unsigned int count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, (const void*)0);
	}
#pragma endregion

#pragma region ImGui Functions
	void OpenGLGraphics::InitializeImGui()
	{
		Graphics::InitializeImGui();

		ImGui_ImplWin32_InitForOpenGL(m_TargetWindow.GetHWND());
		ImGui_ImplOpenGL3_Init("#version 450");
	}
	
	void OpenGLGraphics::NewImGuiFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();

		Graphics::NewImGuiFrame();
	}
	
	void OpenGLGraphics::RenderImGui()
	{
		Graphics::RenderImGui();
		
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	
	void OpenGLGraphics::ShutdownImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		
		Graphics::ShutdownImGui();
	}
#pragma endregion

#pragma region Exceptions
	
	const char* OpenGLGraphics::FramebufferSwapError::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "Failed to swap front and back framebuffers!" << std::endl
			<< GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	const char* OpenGLGraphics::FramebufferSwapError::GetType() const noexcept
	{
		return "OpenGL Graphics Framebuffer Swap Error";
	}

#pragma endregion

}
