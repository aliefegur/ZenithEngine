#include "TestApp.h"
#include <Zenith/Platform/DirectX/D3D11Shader.h>
#include <Zenith/Platform/DirectX/D3D11BatchRenderer.h>
#include <Zenith/Platform/OpenGL/OpenGLShader.h>
#include <Zenith/Platform/OpenGL/OpenGLBatchRenderer.h>

using namespace Zenith;

TestApp::TestApp()
	:
	m_Shader(nullptr), m_BatchRenderer(nullptr), m_Texture(nullptr)
{
	m_Window = new Window(1280, 720, "Zenith Test App", false);
	m_Viewport = new Viewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Camera = new Camera(m_Viewport);
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
	

	/*m_Window->CreateGraphicsContext(Graphics::API::OpenGL);
	m_Shader = new OpenGLShader(m_Window->GetGfx(), "res/shaders/sprite_vs.glsl", "res/shaders/sprite_fs.glsl");
	m_BatchRenderer = new OpenGLBatchRenderer(m_Window->GetGfx(), m_Shader);*/
	m_Texture = Texture2D::LoadWhiteTexture(m_Window->GetGfx());
}

void TestApp::Update()
{
	ZenithApp::Update();

	m_Window->GetGfx()->ClearBuffer(0.2f, 1.0f, 0.3f, 1.0f);

	m_BatchRenderer->Begin();
	m_BatchRenderer->DrawTexture(m_Texture, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	m_BatchRenderer->End();

	m_Window->GetGfx()->EndFrame();
	m_Window->ProcessEvents();

	m_Viewport->SetDimensions(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Viewport->Apply(m_Window->GetGfx());
}
