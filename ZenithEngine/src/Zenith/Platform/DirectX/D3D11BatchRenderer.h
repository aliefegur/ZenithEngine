#pragma once

#include "D3D11Renderer.h"
#include "D3D11Texture2D.h"
#include <Zenith/Renderer/BatchRenderer.h>

namespace Zenith
{
	class D3D11BatchRenderer : public BatchRenderer, public D3D11Renderer
	{
	public:
		D3D11BatchRenderer(Graphics* gfx, Shader* shader);
		~D3D11BatchRenderer();

		void End() override;

		void DrawTexture(Texture2D* texture, const glm::mat4& transform, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }) override;

	private:
		ID3D11Buffer		*pVertexBuffer,
							*pIndexBuffer;
		ID3D11InputLayout	*pInputLayout;
		ID3D11Buffer		*pConstantBuffer;
		D3D11Texture2D		*pTextureSlots[MAX_TEXTURES];
	};
}
