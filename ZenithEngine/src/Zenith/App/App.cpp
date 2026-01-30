#include "zenithpch.h"

#include "App.h"
#include "Zenith/Utils/Time.h"
#include "Zenith/Renderer/UI/FontLoader.h"

namespace Zenith
{
	ZenithApp::ZenithApp() 
		:
		m_Window(nullptr)
	{
		FontLoader::Init();
	}

	ZenithApp::~ZenithApp() 
	{
		FontLoader::Shutdown();
	}

	void ZenithApp::Start() 
	{
	}

	void ZenithApp::Update() 
	{
		Time::Update();
	}

	Window* ZenithApp::GetWindow() const noexcept
	{
		return m_Window;
	}
}
