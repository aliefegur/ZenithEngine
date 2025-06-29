#include "TestApp.h"
#include <Zenith/Platform/DirectX/D3D11Shader.h>
#include <Zenith/Platform/DirectX/D3D11BatchRenderer.h>

using namespace Zenith;

TestApp::TestApp()
	:
	m_Shader(nullptr), m_BatchRenderer(nullptr)
{
	m_Window = new Window(1280, 720, "Zenith Test App", false);
}

TestApp::~TestApp()
{
	delete m_Shader;
	delete m_BatchRenderer;
}

void TestApp::Start()
{
	ZenithApp::Start();

	m_Window->Show();
	m_Window->CreateGraphicsContext(Graphics::API::D3D11);

	m_Shader = new D3D11Shader(m_Window->GetGfx(), "res/shaders/SpriteVS.cso", "res/shaders/SpritePS.cso");
	m_BatchRenderer = new D3D11BatchRenderer(m_Window->GetGfx(), m_Shader);
}

void TestApp::Update()
{
	ZenithApp::Update();

	m_Window->GetGfx()->ClearBuffer(0.2f, 1.0f, 0.3f, 1.0f);

	m_Window->GetGfx()->EndFrame();
	m_Window->ProcessEvents();
}
