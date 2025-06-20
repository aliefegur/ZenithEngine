#include "zenithpch.h"
#include "UIRenderer.h"
#include "Font.h"
#include "../Stats.h"
#include "Zenith/Utils/ZenithException.h"
#include "../../Platform/OpenGL/OpenGLShader.h"

namespace Zenith
{
	extern int				MAX_TEXTURES;
	static constexpr size_t	QUAD_PER_BATCH = 1000;

	UIRenderer::UIRenderer(Graphics* gfx, Shader* textShader, Shader* imageShader)
		:
		m_Gfx(gfx)
	{
		InitializeTextRenderer(textShader);
		InitializeImageRenderer(imageShader);
	}

	UIRenderer::~UIRenderer()
	{
		DestroyTextRenderer();
		DestroyImageRenderer();
	}

	void UIRenderer::Begin(const glm::ivec2& windowDimension)
	{
		BeginTextRenderer();
		BeginImageRenderer();
		m_WindowDimensions = windowDimension;
	}

	void UIRenderer::End()
	{
		EndTextRenderer();
		EndImageRenderer();
	}

	void UIRenderer::DrawImage(Texture2D* img, const glm::vec2& position, const glm::vec2& scale, float rotation, Anchor anchor, Anchor pivot, const glm::vec4& color)
	{
		if (m_ImageIndexCount >= QUAD_PER_BATCH * 6)
		{
			EndImageRenderer();
			BeginImageRenderer();
		}

		// Is the texture has used in the current batch?
		float texIndex = -1.0f;
		for (uint32_t i = 0; i < m_ImageTextureSlotIndex; i++)
		{
			if (m_ImageTextureSlots[i] == img->GetID())
			{
				texIndex = static_cast<float>(i);
				break;
			}
		}

		// If not used, add the id of this texture to the texture slots array
		if (texIndex == -1.0f)
		{
			if (m_ImageTextureSlotIndex >= static_cast<uint32_t>(MAX_TEXTURES) - 1)
			{
				EndImageRenderer();
				BeginImageRenderer();
			}

			texIndex = static_cast<float>(m_ImageTextureSlotIndex);
			m_ImageTextureSlots[m_ImageTextureSlotIndex] = img->GetID();
			++m_ImageTextureSlotIndex;
		}

		glm::vec2 pos{ 0.0f };

		// Set position by anchor
		// x-axis
		switch (static_cast<int>(anchor) % 3)
		{
		case 0:	pos.x = position.x; break;									// LEFT
		case 1:	pos.x = m_WindowDimensions.x / 2.0f + position.x; break;	// CENTER
		case 2:	pos.x = m_WindowDimensions.x - position.x; break;			// RIGHT
		}

		// y-axis
		switch (static_cast<int>(anchor) / 3)
		{
		case 0: pos.y = m_WindowDimensions.y - position.y; break;			// TOP
		case 1:	pos.y = m_WindowDimensions.y / 2.0f + position.y; break;	// MIDDLE
		case 2: pos.y = position.y; break;									// BOTTOM
		}

		// Set vertices by pivot
		glm::vec4 vertexPoses[] = {
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f, 1.0f },
			{ 1.0f, 1.0f, 0.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
		};

		glm::vec2 texCoords[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f },
		};

		// Pivot
		for (glm::vec4& vert : vertexPoses)
		{
			vert -= glm::vec4{
				0.5f * (static_cast<int>(pivot) % 3),
				0.5f * (2 - static_cast<int>(pivot) / 3),
				0.0f,
				0.0f,
			};
		}

		// Transform vertices
		for (size_t i = 0; i < 4; i++)
		{
			m_ImageQuadBufferPtr->Position = glm::scale(
				glm::rotate(
					glm::translate(
						glm::mat4(1.0f),
						{ pos.x, pos.y, 0.0f }
					),
					glm::radians(rotation),
					{ 0.0f, 0.0f, 1.0f }
				),
				{ scale.x, scale.y, 0.0f }
			) * vertexPoses[i];
			m_ImageQuadBufferPtr->Color = color;
			m_ImageQuadBufferPtr->TextureCoords = texCoords[i];
			m_ImageQuadBufferPtr->TextureIndex = texIndex;
			++m_ImageQuadBufferPtr;
		}

		m_ImageIndexCount += 6;

		RendererStats::s_TriangleCount += 2;
		RendererStats::s_VertexCount += 4;
	}

	void UIRenderer::DrawTxt(Font* fontFamily, const std::string& text, const glm::vec2& position, float fontSize, const glm::vec4& color)
	{
		glm::vec2 offset{ 0.0f };

		for (auto& c : text)
		{
			Font::Character ch = fontFamily->m_Characters[c];
			offset.x += ch.Bearing.x * fontSize / (float)fontFamily->m_FontSize;
			offset.y = -(ch.GlyphSize.y - ch.Bearing.y) * fontSize / (float)fontFamily->m_FontSize;
			DrawChar(fontFamily, c, position + offset, fontSize, color);
			offset.x += (ch.Advance >> 6) * fontSize / (float)fontFamily->m_FontSize;
		}
	}

	void UIRenderer::DrawTxt(Font* fontFamily, const std::string& text, const glm::vec2& position, Anchor anchor, float fontSize, const glm::vec4& color)
	{
		const float scaleFactor = fontSize / (float)fontFamily->m_FontSize;

		glm::vec2 textboxDimensions{ 0.0f };
		glm::vec2 textboxPosition;

		textboxDimensions.y = fontFamily->m_FontSize * scaleFactor;

		for (auto& c : text)
		{
			const Font::Character ch = fontFamily->m_Characters[c];
			textboxDimensions.x += (ch.Bearing.x + (ch.Advance >> 6)) * scaleFactor;
		}

		switch (anchor)
		{
		case Zenith::Anchor::TopLeft:
			textboxPosition = {
				position.x,
				m_WindowDimensions.y - position.y - textboxDimensions.y
			};
			break;
		case Zenith::Anchor::TopCenter:
			textboxPosition = {
				m_WindowDimensions.x / 2.0f + position.x - textboxDimensions.x / 2.0f,
				m_WindowDimensions.y - position.y - textboxDimensions.y
			};
			break;
		case Zenith::Anchor::TopRight:
			textboxPosition = {
				m_WindowDimensions.x - position.x - textboxDimensions.x,
				m_WindowDimensions.y - position.y - textboxDimensions.y
			};
			break;
		
		case Zenith::Anchor::MiddleLeft:
			textboxPosition = {
				position.x,
				m_WindowDimensions.y / 2.0f + position.y - textboxDimensions.y / 2.0f
			};
			break;
		case Zenith::Anchor::MiddleCenter:
			textboxPosition = {
				m_WindowDimensions.x / 2.0f + position.x - textboxDimensions.x / 2.0f,
				m_WindowDimensions.y / 2.0f + position.y - textboxDimensions.y / 2.0f
			};
			break;
		case Zenith::Anchor::MiddleRight:
			textboxPosition = {
				m_WindowDimensions.x - position.x - textboxDimensions.x,
				m_WindowDimensions.y / 2.0f + position.y - textboxDimensions.y / 2.0f
			};
			break;
		
		case Zenith::Anchor::BottomLeft:
			textboxPosition = {
				position.x,
				position.y
			};
			break;
		case Zenith::Anchor::BottomCenter:
			textboxPosition = {
				m_WindowDimensions.x / 2.0f + position.x - textboxDimensions.x / 2.0f,
				position.y
			};
			break;
		case Zenith::Anchor::BottomRight:
			textboxPosition = {
				m_WindowDimensions.x - position.x - textboxDimensions.x,
				position.y
			};
			break;
		}
	
		DrawTxt(fontFamily, text, textboxPosition, fontSize, color);
	}

	void UIRenderer::DrawChar(Font* fontFamily, char chr, const glm::vec2& position, float fontSize, const glm::vec4& color)
	{
		if (m_TextIndexCount >= QUAD_PER_BATCH * 6)
		{
			End();
			Begin(m_WindowDimensions);
		}

		Font::Character c = fontFamily->m_Characters[chr];

		// Is the texture has used in the current batch?
		float texIndex = -1.0f;
		for (uint32_t i = 0; i < m_TextTextureSlotIndex; i++)
		{
			if (m_TextTextureSlots[i] == c.TextureID)
			{
				texIndex = static_cast<float>(i);
				break;
			}
		}

		// If not used, add the id of this texture to the texture slots array
		if (texIndex == -1.0f)
		{
			if (m_TextTextureSlotIndex >= static_cast<uint32_t>(MAX_TEXTURES) - 1)
			{
				End();
				Begin(m_WindowDimensions);
			}

			texIndex = static_cast<float>(m_TextTextureSlotIndex);
			m_TextTextureSlots[m_TextTextureSlotIndex] = c.TextureID;
			++m_TextTextureSlotIndex;
		}

		// Initial vertex data
		float width = ((float)c.GlyphSize.x * fontSize / fontFamily->m_FontSize);
		float height = ((float)c.GlyphSize.y * fontSize / fontFamily->m_FontSize);

		glm::vec4 vertexPoses[] = {
			{ 0.0f, 0.0f, 0.0f, 1.0f },		// BOTTOM-LEFT
			{ width, 0.0f, 0.0f, 1.0f },	// BOTTOM-RIGHT
			{ width, height, 0.0f, 1.0f },	// TOP-RIGHT
			{ 0.0f,  height, 0.0f, 1.0f },	// TOP-LEFT
		};
		glm::vec2 texCoords[] = {
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 0.0f },
			{ 0.0f, 0.0f }
		};

		// Transform vertices
		for (size_t i = 0; i < 4; i++)
		{
			m_TextQuadBufferPtr->Position = glm::translate(glm::mat4(1.0f), {(float)position.x, (float)position.y, 0.0f}) * vertexPoses[i];
			m_TextQuadBufferPtr->Color = color;
			m_TextQuadBufferPtr->TextureCoords = texCoords[i];
			m_TextQuadBufferPtr->TextureIndex = texIndex;
			++m_TextQuadBufferPtr;
		}

		m_TextIndexCount += 6;

		RendererStats::s_TriangleCount += 2;
		RendererStats::s_VertexCount += 4;
	}

	void UIRenderer::InitializeTextRenderer(Shader* textShader)
	{
		m_TextShader = textShader;
		m_TextQuadBuffer = new Vertex[QUAD_PER_BATCH];

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
		glCreateVertexArrays(1, &m_TextVAO);
		glCreateBuffers(1, &m_TextVBO);
		glCreateBuffers(1, &m_TextEBO);

		// Bind OpenGL buffers
		glBindVertexArray(m_TextVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_TextVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_TextEBO);

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

		// Texture slots initialization
		m_TextTextureSlots = new uint32_t[MAX_TEXTURES];
		for (size_t i = 0; i < MAX_TEXTURES; i++)
			m_TextTextureSlots[i] = 0;
		m_TextTextureSlotIndex = 0;
	}

	void UIRenderer::InitializeImageRenderer(Shader* imageShader)
	{
		m_ImageShader = imageShader;
		m_ImageQuadBuffer = new Vertex[QUAD_PER_BATCH];

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
		glCreateVertexArrays(1, &m_ImageVAO);
		glCreateBuffers(1, &m_ImageVBO);
		glCreateBuffers(1, &m_ImageEBO);

		// Bind OpenGL buffers
		glBindVertexArray(m_ImageVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_ImageVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ImageEBO);

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

		// Texture slots initialization
		m_ImageTextureSlots = new uint32_t[MAX_TEXTURES];
		for (size_t i = 0; i < MAX_TEXTURES; i++)
			m_ImageTextureSlots[i] = 0;
		m_ImageTextureSlotIndex = 0;
	}

	void UIRenderer::DestroyTextRenderer()
	{
		glDeleteVertexArrays(1, &m_TextVAO);
		glDeleteBuffers(1, &m_TextVBO);
		glDeleteBuffers(1, &m_TextEBO);

		delete[] m_TextTextureSlots;
		delete[] m_TextQuadBuffer;
	}
	
	void UIRenderer::DestroyImageRenderer()
	{
		glDeleteVertexArrays(1, &m_ImageVAO);
		glDeleteBuffers(1, &m_ImageVBO);
		glDeleteBuffers(1, &m_ImageEBO);

		delete[] m_ImageTextureSlots;
		delete[] m_ImageQuadBuffer;
	}

	void UIRenderer::BeginTextRenderer()
	{
		m_TextQuadBufferPtr = m_TextQuadBuffer;
	}

	void UIRenderer::BeginImageRenderer()
	{
		m_ImageQuadBufferPtr = m_ImageQuadBuffer;
	}

	void UIRenderer::EndTextRenderer()
	{
		// Load vertex data to the GPU
		GLsizeiptr size = (uint8_t*)m_TextQuadBufferPtr - (uint8_t*)m_TextQuadBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, m_TextVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, (const void*)m_TextQuadBuffer);

		// Activate shader pipeline
		m_TextShader->Bind(m_Gfx);

		// Bind texture layer
		int* samplers = new int[m_TextTextureSlotIndex];
		for (uint32_t i = 0; i < m_TextTextureSlotIndex; i++)
		{
			glBindTextureUnit(i, m_TextTextureSlots[i]);
			samplers[i] = i;
		}

		OpenGLShader* textShader = reinterpret_cast<OpenGLShader*>(m_TextShader);

		// Update uniform variables
		textShader->SetUniformVec2("u_WindowSize", { static_cast<float>(m_WindowDimensions.x), static_cast<float>(m_WindowDimensions.y) });
		textShader->SetUniformIntArray("u_Samplers", samplers, m_TextTextureSlotIndex);

		// Binding
		glBindVertexArray(m_TextVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_TextEBO);

		// Drawing
		glDrawElements(GL_TRIANGLES, m_TextIndexCount, GL_UNSIGNED_SHORT, (const void*)0);

		m_TextIndexCount = 0;
		m_TextTextureSlotIndex = 0;

		delete[] samplers;

		++RendererStats::s_BatchCount;
	}

	void UIRenderer::EndImageRenderer()
	{
		// Load vertex data to the GPU
		GLsizeiptr size = (uint8_t*)m_ImageQuadBufferPtr - (uint8_t*)m_ImageQuadBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, m_ImageVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, (const void*)m_ImageQuadBuffer);

		// Activate shader pipeline
		m_ImageShader->Bind(m_Gfx);

		// Bind texture layer
		int* samplers = new int[m_ImageTextureSlotIndex];
		for (uint32_t i = 0; i < m_ImageTextureSlotIndex; i++)
		{
			glBindTextureUnit(i, m_ImageTextureSlots[i]);
			samplers[i] = i;
		}

		OpenGLShader* imageShader = reinterpret_cast<OpenGLShader*>(m_ImageShader);

		// Update uniform variables
		imageShader->SetUniformVec2("u_WindowSize", { static_cast<float>(m_WindowDimensions.x), static_cast<float>(m_WindowDimensions.y) });
		imageShader->SetUniformIntArray("u_Samplers", samplers, m_ImageTextureSlotIndex);

		// Binding
		glBindVertexArray(m_ImageVAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ImageEBO);

		// Drawing
		glDrawElements(GL_TRIANGLES, m_ImageIndexCount, GL_UNSIGNED_SHORT, (const void*)0);

		m_ImageIndexCount = 0;
		m_ImageTextureSlotIndex = 0;

		delete[] samplers;

		++RendererStats::s_BatchCount;
	}
}
