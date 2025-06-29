#pragma once

#include <ZenithEngine.h>

class TestApp : public Zenith::ZenithApp
{
public:
	TestApp();
	~TestApp();
	void Start() override;
	void Update() override;

private:
	Zenith::Shader* m_Shader;
	Zenith::BatchRenderer* m_BatchRenderer;
};
