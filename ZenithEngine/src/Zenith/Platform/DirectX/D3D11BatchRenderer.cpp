#include "zenithpch.h"
#include "D3D11BatchRenderer.h"
#include "D3D11Graphics.h"
#include "D3D11Shader.h"

namespace Zenith
{
	D3D11BatchRenderer::D3D11BatchRenderer(Graphics* _gfx, Shader* shader)
		:
		BatchRenderer(_gfx, shader)
	{
		D3D11Graphics* gfx = reinterpret_cast<D3D11Graphics*>(_gfx);
		
		INFO_MANAGER(gfx);

		// Calculate indices
		const size_t indexCount = QUAD_PER_BATCH * 6;
		unsigned short indices[indexCount] = {};
		uint16_t offset = 0;
		for (size_t i = 0; i < indexCount; i += 6)
		{
			indices[i] = offset;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset;
			indices[i + 4] = offset + 2;
			indices[i + 5] = offset + 3;
			offset += 4;
		}

		// Create vertex buffer
		D3D11_BUFFER_DESC vbd = {};
		vbd.ByteWidth = sizeof(Vertex) * QUAD_PER_BATCH;
		vbd.StructureByteStride = sizeof(Vertex);
		vbd.Usage = D3D11_USAGE_DYNAMIC;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbd.MiscFlags = NULL;
		GFX_THROW(GetDevice(gfx)->CreateBuffer(&vbd, nullptr, &pVertexBuffer));

		// Create index buffer
		D3D11_BUFFER_DESC ibd = {};
		ibd.ByteWidth = sizeof(indices);
		ibd.StructureByteStride = sizeof(unsigned short);
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = NULL;
		ibd.MiscFlags = NULL;
		
		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices;
		GFX_THROW(GetDevice(gfx)->CreateBuffer(&ibd, &isd, &pIndexBuffer));

		// Get Shader
		D3D11Shader* d3dshader = reinterpret_cast<D3D11Shader*>(m_ShaderProgram);

		// Create Input Layout
		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, Color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, TextureCoords), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXINDEX", 0, DXGI_FORMAT_R32_FLOAT, 0, offsetof(Vertex, TextureIndex), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		GFX_THROW(GetDevice(gfx)->CreateInputLayout(
			ied,
			std::size(ied),
			d3dshader->GetVSBytecode()->GetBufferPointer(),
			d3dshader->GetVSBytecode()->GetBufferSize(),
			&pInputLayout
		));
	}

	D3D11BatchRenderer::~D3D11BatchRenderer()
	{
		pVertexBuffer->Release();
		pIndexBuffer->Release();
		pInputLayout->Release();
	}
	
	void D3D11BatchRenderer::End()
	{
	}
	
	void D3D11BatchRenderer::DrawTexture(Texture2D* texture, const glm::mat4& transform, const glm::vec4& color)
	{
	}
}
