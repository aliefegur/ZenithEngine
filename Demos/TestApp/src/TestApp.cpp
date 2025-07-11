#include "TestApp.h"

using namespace Zenith;

TestApp::TestApp()
	:
	m_Shader(nullptr), m_BatchRenderer(nullptr), m_Texture(nullptr)
{
	m_Window = new Window(1280, 720, "Zenith Test App", false);
	m_Viewport = new Viewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Camera = new Camera(m_Viewport);
	m_Camera->zNear = 0.0f;
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
	
	constexpr Graphics::API api = Graphics::API::D3D11; // Change to OpenGL if needed
	m_Window->CreateGraphicsContext(api);
	m_Shader = Shader::LoadShader(
		m_Window->GetGfx(), 
		api == Graphics::API::D3D11 ? "res/shaders/SpriteVS.cso" : "res/shaders/sprite_vs.glsl",
		api == Graphics::API::D3D11 ? "res/shaders/SpritePS.cso" : "res/shaders/sprite_fs.glsl"
	);
	m_BatchRenderer = BatchRenderer::Create(m_Window->GetGfx(), m_Shader);
	
	m_Texture = Texture2D::LoadWhiteTexture(m_Window->GetGfx());

	Input::SetupEventWindow(m_Window);
	m_Window->BindEventListener(this);
}

void TestApp::Update()
{
	ZenithApp::Update();

	m_Camera->Update();

	// Rendering
	{
		m_Window->GetGfx()->ClearBuffer(0.2f, 1.0f, 0.3f, 1.0f);

		float _x = 2.5f;
		float _h = 2.0f;
		float scale = 0.15f;
		int i = 0;

		m_BatchRenderer->Begin(m_Camera->GetCombinedMatrix());

		for (float x = -_x; x < _x; x += scale)
		{
			for (float y = -_h; y < _h; y += scale)
			{
				m_BatchRenderer->DrawTexture(
					m_Texture,
					glm::vec3(x, y, 0.0f),
					glm::vec3(scale, scale, 1.0f),
					++i % 2 == 0 ? glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) : glm::vec4(0.75f, 0.75f, 0.75f, 1.0f)
				);
			}
		}

		m_BatchRenderer->End();

		ImGui::Begin("Rendering Stats");
		ImGui::Text("API: %s", m_Window->GetGfx()->GetAPIType() == Graphics::API::D3D11 ? "D3D11" : "OpenGL");
		ImGui::Text("Verts: %d", RendererStats::GetVertexCount());
		ImGui::Text("Tris: %d", RendererStats::GetTriangleCount());
		ImGui::Text("Batches: %d", RendererStats::GetBatchCount());
		ImGui::End();

		m_Window->GetGfx()->EndFrame();
	}
	
	m_Window->ProcessEvents();
}

void TestApp::OnWindowResize(Window* window, int width, int height)
{
	m_Viewport->SetDimensions(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Viewport->Apply(m_Window->GetGfx());
}
