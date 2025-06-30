#pragma once

#include <ZenithEngine.h>

class TestApp : public Zenith::ZenithApp, public Zenith::EventListener
{
public:
	TestApp();
	~TestApp();
	void Start() override;
	void Update() override;

private:
	void OnWindowResize(Zenith::Window* window, int width, int height) override;

private:
	Zenith::Shader* m_Shader;
	Zenith::BatchRenderer* m_BatchRenderer;
	Zenith::Camera* m_Camera;
	Zenith::Viewport* m_Viewport;
	Zenith::Texture2D* m_Texture;
	bool m_RenderingEnabled = true;
};
