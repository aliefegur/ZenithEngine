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

		// Create constant buffer
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = NULL;
		cbd.ByteWidth = sizeof(glm::mat4);
		cbd.StructureByteStride = NULL;
		GFX_THROW(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
	}

	D3D11BatchRenderer::~D3D11BatchRenderer()
	{
		pVertexBuffer->Release();
		pIndexBuffer->Release();
		pInputLayout->Release();
		pConstantBuffer->Release();
	}
	
    void D3D11BatchRenderer::End()  
    {  
        D3D11Graphics* gfx = reinterpret_cast<D3D11Graphics*>(m_Gfx);  
        INFO_MANAGER(gfx);  

        // Load vertex data to the VRAM  
        D3D11_MAPPED_SUBRESOURCE mappedResource;  
        GFX_THROW(GetContext(gfx)->Map(pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));  
        memcpy(mappedResource.pData, m_QuadBuffer, (uint8_t*)m_QuadbufferPtr - (uint8_t*)m_QuadBuffer);  
        GetContext(gfx)->Unmap(pVertexBuffer, 0);  

        // Bind input layout  
        GetContext(gfx)->IASetInputLayout(pInputLayout);  

        // Bind vertex buffer  
        const UINT stride = sizeof(Vertex);  
        const UINT offset = 0;  
        GetContext(gfx)->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);  

        // Bind index buffer  
        GetContext(gfx)->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);  

        // Set primitive topology  
        GetContext(gfx)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  

        // Bind shader  
        reinterpret_cast<D3D11Shader*>(m_ShaderProgram)->Bind(m_Gfx);  

        // Bind textures and sampler states  
        ID3D11ShaderResourceView* textures[MAX_TEXTURES] = {};  
        ID3D11SamplerState* samplers[MAX_TEXTURES] = {};  
		
		for (size_t i = 0; i < m_TextureSlotIndex; ++i)  
		{
			D3D11Texture2D* tex = pTextureSlots[i];
			textures[i] = tex->GetShaderResourceView();
			samplers[i] = tex->GetSamplerState();
		}

        GetContext(gfx)->PSSetShaderResources(0, m_TextureSlotIndex, textures);  
        GetContext(gfx)->PSSetSamplers(0, m_TextureSlotIndex, samplers);

		// Update constant buffer with view-projection matrix
		D3D11_MAPPED_SUBRESOURCE csd = {};
		GetContext(gfx)->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &csd);
		memcpy(csd.pData, glm::value_ptr(m_ViewProj), sizeof(m_ViewProj));
		GetContext(gfx)->Unmap(pConstantBuffer, 0);

		// Bind constant buffer into the vertex shader
		GetContext(gfx)->VSSetConstantBuffers(0, 1, &pConstantBuffer);

		// Draw indexed triangles
		gfx->DrawIndexed(m_IndexCount);

        BatchRenderer::End();
    }
	
	void D3D11BatchRenderer::DrawTexture(Texture2D* _texture, const glm::mat4& transform, const glm::vec4& color)
	{
		BatchRenderer::DrawTexture(_texture, transform, color);

		// The texture used in the current batch?
		float texIndex = -1.0f;
		D3D11Texture2D* texture = reinterpret_cast<D3D11Texture2D*>(_texture);
		for (size_t i = 0; i < m_TextureSlotIndex; ++i)
		{
			if (pTextureSlots[i] == texture)
			{
				texIndex = static_cast<float>(i);
				break;
			}
		}

		// If not used, add it to texture slots array
		if (texIndex == -1.0f)
		{
			if (m_TextureSlotIndex >= static_cast<uint32_t>(MAX_TEXTURES) - 1)
			{
				End();
				Begin(m_ViewProj);
			}

			pTextureSlots[m_TextureSlotIndex++] = texture;
			texIndex = static_cast<float>(m_TextureSlotIndex - 1);
		}

		// Initial vertex data
		float right = ((float)texture->GetWidth() / texture->PixelPerUnit()) / 2.0f;
		float top = ((float)texture->GetHeight() / texture->PixelPerUnit()) / 2.0f;
		glm::vec4 vertexPoses[] = {
			{ -right, -top, 0.0f, 1.0f }, // 0: bottom-left
			{ -right,  top, 0.0f, 1.0f }, // 1: top-left
			{  right,  top, 0.0f, 1.0f }, // 2: top-right
			{  right, -top, 0.0f, 1.0f }, // 3: bottom-right
		};
		glm::vec2 texCoords[] = {
			{ 0.0f, 1.0f },
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f }
		};

		// Calculate each vertex
		for (size_t i = 0; i < 4; i++)
		{
			m_QuadbufferPtr->Position = transform * vertexPoses[i];
			m_QuadbufferPtr->Color = color;
			m_QuadbufferPtr->TextureCoords = texCoords[i];
			m_QuadbufferPtr->TextureIndex = texIndex;
			++m_QuadbufferPtr;
		}
	}
}
