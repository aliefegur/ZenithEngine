#include "zenithpch.h"
#include "Shader.h"
#include "Zenith/Platform/OpenGL/OpenGLShader.h"
#include "Zenith/Platform/DirectX/D3D11Shader.h"

namespace Zenith
{
	Shader* Shader::LoadShader(Graphics* gfx, const std::string& vsFile, const std::string& psFile)
	{
		switch (gfx->GetAPIType())
		{
#if ZENITH_PLATFORM_WINDOWS
		case Graphics::API::D3D11:
			return new D3D11Shader(gfx, vsFile, psFile);
#endif
		case Graphics::API::OpenGL:
			return new OpenGLShader(gfx, vsFile, psFile);
		default:
			// TODO: Throw an exception for unsupported API
			break;
		}
	}

	Shader::Shader(Graphics* gfx, const std::string& vsFile, const std::string& psFile)
		:
		Bindable()
	{ }
}
