#include "zenithpch.h"
#include "Graphics.h"
#include "Zenith/App/Window.h"

namespace Zenith 
{
	Graphics::Graphics(Window& targetWindow) 
		: 
		m_TargetWindow(targetWindow),
		m_CurrentAPI(API::None)
	{ }

	Graphics::~Graphics() { }
	
	Graphics::API Graphics::GetAPIType() const noexcept
	{
		return m_CurrentAPI;
	}

	void Graphics::EndFrame() { }
	void Graphics::ClearBuffer(float red, float green, float blue, float alpha) noexcept { }
	void Graphics::DrawIndexed(unsigned int count) { }
}
