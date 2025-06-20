#include "FlappyBirdApp.h"
#include <ctime>
#include <cstdlib>

using namespace Zenith;
using namespace glm;

FlappyBirdApp::FlappyBirdApp()
	:
	m_Shader(nullptr), m_Renderer(nullptr), m_Viewport(nullptr), m_Camera(nullptr), m_BackgroundTexture(nullptr), m_Bg(nullptr), 
	m_Bird(nullptr), m_PipeTexture(nullptr), m_PixelifySans(nullptr), m_Righteous(nullptr), m_UiRenderer(nullptr), m_TextShader(nullptr),
	m_UiImageShader(nullptr)
{
	m_Window = new Window(800, 600, "DirectX 11 Context", false);

	// Read high score
	try
	{
		uint8_t* data = Zenith::File::ReadBinaryFile("game.dat");
		m_HiScore = *(unsigned int*)(data);
	}
	catch (const std::exception&)
	{
		m_HiScore = 0;
	}
}

FlappyBirdApp::~FlappyBirdApp()
{
	// Shutdown ImGui
	ImGuiManager::Shutdown();

	// Delete game objects
	delete m_Bg;
	delete m_Bird;
	for (auto p : m_Pipes) delete p;
	m_Pipes = {};

	// Delete textures
	for (auto charTex : m_CharacterTextures) delete charTex;
	m_CharacterTextures = {};
	delete m_BackgroundTexture;
	delete m_PipeTexture;
	delete m_WhiteTexture;

	// Delete fonts
	delete m_PixelifySans;

	// Unload sound effects
	delete m_DieSfx;
	delete m_PointSfx;
	delete m_SwooshSfx;
	delete m_ThemeSong;

	// Delete audio components
	delete m_UiAudioSource;
	delete m_DieAudioSource;
	delete m_ScoreAudioSource;
	delete m_ThemeAudioSource;

	// Delete rendering components
	delete m_Camera;
	delete m_Viewport;
	delete m_Shader;
	delete m_TextShader;
	delete m_UiImageShader;
	delete m_UiRenderer;
	delete m_Renderer;
	delete m_Window;	
}

void FlappyBirdApp::Start()
{
	ZenithApp::Start();

	Input::SetupEventWindow(m_Window);

	m_Window->Show();
	m_Window->CreateGraphicsContext(Graphics::API::OpenGL);
	
	// Initialize ImGui
	ImGuiManager::Initialize(m_Window);
	/*
	// Create rendering components
	m_Shader = new Shader("res/shaders/sprite_vs.glsl", "res/shaders/sprite_fs.glsl");
	m_TextShader = new Shader("res/shaders/ui_vs.glsl", "res/shaders/text_fs.glsl");
	m_UiImageShader = new Shader("res/shaders/ui_vs.glsl", "res/shaders/sprite_fs.glsl");
	m_Renderer = new BatchRenderer(m_Shader);
	m_UiRenderer = new UIRenderer(m_TextShader, m_UiImageShader);
	m_Viewport = new Viewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Camera = new Camera(m_Viewport);
	m_Camera->zNear = 0.0f;
	*/
	// Loading textures
	m_BackgroundTexture = new Texture2D("res/sprites/background-day.png");
	m_PipeTexture = new Texture2D("res/sprites/pipe-green.png");
	m_CharacterTextures = {
		new Texture2D("res/sprites/yellowbird-downflap.png", 100u, false, Texture2D::Filter::Point),
		new Texture2D("res/sprites/yellowbird-midflap.png", 100u, false, Texture2D::Filter::Point),
		new Texture2D("res/sprites/yellowbird-upflap.png", 100u, false, Texture2D::Filter::Point),
	};
	m_WhiteTexture = Texture2D::LoadWhiteTexture();

	// Loading fonts
	m_PixelifySans = new Font("res/fonts/PixelifySans.ttf", 128u, Texture2D::Filter::Point);
	m_Righteous = new Font("res/fonts/Righteous.ttf", 128u);

	// Initialize audio components
	m_AudioDevice = AudioDevice::Get();
	m_UiAudioSource = new AudioSource();
	m_DieAudioSource = new AudioSource();
	m_ScoreAudioSource = new AudioSource();
	m_ScoreAudioSource->SetGain(0.21f);
	m_ThemeAudioSource = new AudioSource();
	m_ThemeAudioSource->SetGain(0.75f);
	m_ThemeAudioSource->SetLoop(true);

	// Load sound effects
	m_DieSfx = new AudioClip("res/audio/die.ogg");
	m_PointSfx = new AudioClip("res/audio/point.ogg");
	m_SwooshSfx = new AudioClip("res/audio/swoosh.ogg");
	m_ThemeSong = new AudioClip("res/audio/theme.mp3");

	// Create game objects
	m_Bg = new Background(m_BackgroundTexture, { -0.3f, 0.0f, 0.0f });
	m_Bird = new Bird(&m_CharacterTextures[0]);
	m_Pipes = {
		new Pipe(m_PipeTexture, 5.0f,  { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 7.0f,  { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 9.0f,  { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 11.0f, { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 13.0f, { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 15.0f, { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 17.0f, { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 19.0f, { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 21.0f, { -1.0f, 0.0f, 0.0f }, m_Bird)
	};

	// Random seed
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Start theme music
	m_ThemeAudioSource->Play(m_ThemeSong);
}
  
void FlappyBirdApp::Update()
{
	ZenithApp::Update();
	/*
	// Game logic update	
	if (m_GameRunning)
	{
		// Game is running
		m_Bg->Update();
		m_Bird->Update(m_ElapsedTime);

		m_BirdNearPipes = false;
		for (Pipe* pipe : m_Pipes)
		{
			pipe->Update(std::bind(&FlappyBirdApp::OnBirdBetweenPipes, this, std::placeholders::_1));
		}

		if (!m_BirdNearPipes) m_BirdBetweenGaps = false;

		if (abs(m_Bird->Position.y) > 2.4f) ResetGame();
	}
	else
	{
		if (m_FirstLaunched) m_Bg->Update();

		// Game paused
		if (Input::IsKeyJustPressed(Key::Enter))
		{
			StartGame();
		}
	}

	m_Camera->Update();
	*/
	// Render game
	m_Window->GetGfx()->ClearBuffer(1.0f, 0.0f, 0.0f, 1.0f);
	/*
	RenderObjects();
	RenderUI();

#ifdef _DEBUG
	RenderImGui();
#endif
	*/
	m_Window->GetGfx()->EndFrame();
	m_Window->ProcessEvents();
	/*
	m_Viewport->SetDimensions(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Viewport->Apply();

	m_ElapsedTime += Time::Delta();
	*/
}

void FlappyBirdApp::StartGame()
{
	m_UiAudioSource->Play(m_SwooshSfx);

	m_FirstLaunched = false;
	m_GameRunning = true;
	m_BirdBetweenGaps = false;
	m_BirdNearPipes = false;

	// Reset pipe locations
	for (size_t i = 0; i < std::size(m_Pipes); i++)
	{
		m_Pipes[i]->Position.x = 5.0f + 2.0f * i;
	}

	// Start bird
	m_Bird->Start();
}

void FlappyBirdApp::ResetGame()
{
	m_GameRunning = false;
	
	if (m_Score > m_HiScore)
	{
		m_HiScore = m_Score;

		// Save new high score
		File::WriteBinaryFile("game.dat", &m_HiScore, sizeof(m_HiScore));
	}

	m_Score = 0;

	m_DieAudioSource->Play(m_DieSfx);
}

void FlappyBirdApp::RenderObjects()
{
	m_Renderer->Begin(m_Camera->GetCombinedMatrix());
	m_Bg->Render(m_Renderer);
	if (!m_FirstLaunched)
	{
		m_Bird->Render(m_Renderer);
		for (Pipe* pipe : m_Pipes)
			pipe->Render(m_Renderer);
	}
	m_Renderer->End();
}

void FlappyBirdApp::RenderUI()
{
	m_UiRenderer->Begin({ m_Window->GetWidth(), m_Window->GetHeight() });

	// Render menu UI when the game is not running
	if (!m_GameRunning)
	{
		// Draw background panel
		m_UiRenderer->DrawImage(
			m_WhiteTexture,
			{ 0.0f, 0.0f },
			{ m_Window->GetWidth() < 800 ? 480 : m_Window->GetWidth() * 0.6, m_Window->GetHeight() },
			0.0f,
			Anchor::MiddleCenter, Anchor::MiddleCenter,
			{ 0.1, 0.1, 0.1, 0.3 }
		);

		m_UiRenderer->DrawTxt(m_Righteous, "Flappy Bird", { 0.0f, 64.0f }, Anchor::TopCenter, 64.0f);
		m_UiRenderer->DrawTxt(m_PixelifySans, "Hi. Score: " + std::to_string(m_HiScore), { 0.0f, 64.0f }, Anchor::BottomCenter, 20.0f);

		if (m_FirstLaunched)
		{
			m_UiRenderer->DrawImage(m_CharacterTextures[static_cast<int>(m_ElapsedTime * 5) % 3], { 0.0f, 48.0f }, { 160.0f, 128.0f });
			m_UiRenderer->DrawTxt(m_PixelifySans, "Ali Efe GÜR", { 4.0, 4.0 }, Anchor::BottomLeft, 16.0f, { 0.25f, 0.25f, 0.25f, 0.8f });
			m_UiRenderer->DrawTxt(m_PixelifySans, "v1.0.0", { 4.0, 4.0 }, Anchor::BottomRight, 12.0f, { 0.25f, 0.25f, 0.25f, 0.8f });
			m_UiRenderer->DrawTxt(m_PixelifySans, "Press ENTER to Start Game", { 0.0f, -96.0f }, Anchor::MiddleCenter, 20.0f, { 0.05f, 0.05f, 0.05f, abs(sinf(m_ElapsedTime * 1.75f)) * 0.8f + 0.2f });
		}
		else
		{
			m_UiRenderer->DrawTxt(m_PixelifySans, "Press ENTER to Restart Game", { 0.0f, -96.0f }, Anchor::MiddleCenter, 20.0f, { 0.05f, 0.05f, 0.05f, abs(sinf(m_ElapsedTime * 1.75f)) * 0.8f + 0.2f });
		}
	}

	// Render in-game UI when the game is running
	else
	{
		m_UiRenderer->DrawTxt(m_Righteous, std::to_string(m_Score), { 0.0f, 16.0f }, Anchor::TopCenter, 96.0f);
	}
	
	m_UiRenderer->End();
}

void FlappyBirdApp::RenderImGui()
{
	ImGuiManager::NewFrame();
	ImGui::Begin("Rendering Stats");

	ImGui::Text("FPS: %g (%g ms)", 1.0f / Time::Delta(), 1000.0f * Time::Delta());

	// Update FPS plot
	for (int i = 1; i < std::size(m_FpsHistory); i++)
	{
		m_FpsHistory[i - 1] = m_FpsHistory[i];
	}
	m_FpsHistory[std::size(m_FpsHistory) - 1] = 1.0f / Time::Delta();
	ImGui::PlotLines("FPS", m_FpsHistory, (int)std::size(m_FpsHistory));

	ImGui::Text("Batch Count: %llu", RendererStats::GetBatchCount());
	ImGui::Text("Vertex Count: %llu", RendererStats::GetVertexCount());
	ImGui::Text("Traingle Count: %llu", RendererStats::GetTriangleCount());

	ImGui::End();
	ImGuiManager::EndFrame();
}

void FlappyBirdApp::OnBirdBetweenPipes(bool isCollide)
{
	m_BirdNearPipes = true;

	if (isCollide)
	{
		ResetGame();
	}
	else
	{
		if (!m_BirdBetweenGaps)
		{
			++m_Score;
			m_BirdBetweenGaps = true;
			m_ScoreAudioSource->Play(m_PointSfx);
		}
	}
}
