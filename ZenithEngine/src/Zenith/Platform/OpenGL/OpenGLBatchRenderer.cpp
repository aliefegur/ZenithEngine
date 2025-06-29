#include "zenithpch.h"
#include "OpenGLBatchRenderer.h"
#include "OpenGLTexture2D.h"
#include "OpenGLShader.h"

namespace Zenith
{
	extern int MAX_TEXTURES;

	OpenGLBatchRenderer::OpenGLBatchRenderer(Graphics* gfx, Shader* shader)
		:
		BatchRenderer(gfx, shader)
	{
		// Calculate indices
		const size_t indexCount = QUAD_PER_BATCH * 6;
		GLushort* indices = new GLushort[indexCount];
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

		// Generate OpenGL buffers
		glCreateVertexArrays(1, &m_VAO);
		glCreateBuffers(1, &m_VBO);
		glCreateBuffers(1, &m_EBO);

		// Bind OpenGL buffers
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

		// Load OpenGL buffer datas
		glBufferData(GL_ARRAY_BUFFER, QUAD_PER_BATCH * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLushort), indices, GL_STATIC_DRAW);

		// Vertex attributes
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TextureCoords));
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TextureIndex));

		// Unbind buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Delete indices array
		delete[] indices;
	}
	
	OpenGLBatchRenderer::~OpenGLBatchRenderer()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
	}
	
	void OpenGLBatchRenderer::End()
	{
		// Load vertex data to the VRAM
		GLsizeiptr size = (uint8_t*)m_QuadbufferPtr - (uint8_t*)m_QuadBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, (const char*)m_QuadBuffer);

		OpenGLShader* shaderProgram = reinterpret_cast<OpenGLShader*>(m_ShaderProgram);

		// Activate shader pipeline
		shaderProgram->Bind();

		// Bind texture layer
		int* samplers = new int[m_TextureSlotIndex];
		for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
		{
			glBindTextureUnit(i, m_TextureSlots[i]);
			samplers[i] = i;
		}

		// Update uniform variables
		shaderProgram->SetUniformMatrix4("u_ViewProj", m_ViewProj);
		shaderProgram->SetUniformIntArray("u_Samplers", samplers, m_TextureSlotIndex);

		// Bind
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_SHORT, (const void*)0);

		delete[] samplers;

		BatchRenderer::End();
	}
	
	void OpenGLBatchRenderer::DrawTexture(Texture2D* _texture, const glm::mat4& transform, const glm::vec4& color)
	{
		BatchRenderer::DrawTexture(_texture, transform, color);

		OpenGLTexture2D* texture = reinterpret_cast<OpenGLTexture2D*>(_texture);

		// Is the texture has used in the current batch?
		float texIndex = -1.0f;
		for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
		{
			if (m_TextureSlots[i] == texture->GetID())
			{
				texIndex = static_cast<float>(i);
				break;
			}
		}

		// If not used, add the id of this texture to the texture slots array
		if (texIndex == -1.0f)
		{
			if (m_TextureSlotIndex >= static_cast<uint32_t>(MAX_TEXTURES) - 1)
			{
				End();
				Begin(m_ViewProj);
			}

			texIndex = static_cast<float>(m_TextureSlotIndex);
			m_TextureSlots[m_TextureSlotIndex] = texture->GetID();
			++m_TextureSlotIndex;
		}

		// Initial vertex data
		float right = ((float)texture->GetWidth() / texture->PixelPerUnit()) / 2.0f;
		float top = ((float)texture->GetHeight() / texture->PixelPerUnit()) / 2.0f;
		glm::vec4 vertexPoses[] = {
			{ -right, -top, 0.0f, 1.0f },
			{  right, -top, 0.0f, 1.0f },
			{  right,  top, 0.0f, 1.0f },
			{ -right,  top, 0.0f, 1.0f }
		};
		glm::vec2 texCoords[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
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
