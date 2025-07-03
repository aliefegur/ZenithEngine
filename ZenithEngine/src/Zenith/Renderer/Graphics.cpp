#include "zenithpch.h"
#include "Graphics.h"
#include "Zenith/App/Window.h"

namespace Zenith 
{
	Graphics::Graphics(Window& targetWindow) 
		: 
		m_TargetWindow(targetWindow),
		m_CurrentAPI(API::None)
	{
	}

	Graphics::~Graphics() 
	{
		ShutdownImGui();
	}
	
	Graphics::API Graphics::GetAPIType() const noexcept
	{
		return m_CurrentAPI;
	}

	void Graphics::EndFrame() 
	{
		RenderImGui();
	}
	
	void Graphics::ClearBuffer(float red, float green, float blue, float alpha) noexcept 
	{
		NewImGuiFrame();
	}
	
	void Graphics::DrawIndexed(unsigned int count) { }
	
	void Graphics::InitializeImGui() 
	{ 
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// Enable Gamepad Control
		// TODO: Think about enabling docking
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	}

	void Graphics::NewImGuiFrame()
	{
		ImGui::NewFrame();
	}

	void Graphics::RenderImGui()
	{
		ImGui::Render();
	}

	void Graphics::ShutdownImGui()
	{
		ImGui::DestroyContext();
	}
}
