#pragma once

#include "Zenith/Renderer/Shader.h"
#include "D3D11Bindable.h"

namespace Zenith
{
	class D3D11Shader : public Shader, public D3D11Bindable
	{
	public:
		D3D11Shader(Graphics* gfx, const std::string& vsFile, const std::string& psFile);
		~D3D11Shader();

		void Bind(Graphics* gfx) noexcept override;

		ID3DBlob* GetVSBytecode() const noexcept;
		ID3DBlob* GetPSBytecode() const noexcept;

	private:
		ID3D11VertexShader* pVertexShader;
		ID3D11PixelShader* pPixelShader;
		ID3DBlob *pVsbc, *pPsbc;
	};
}
