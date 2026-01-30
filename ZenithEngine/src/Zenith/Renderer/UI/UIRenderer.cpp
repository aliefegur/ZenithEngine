#include "zenithpch.h"
#include "UIRenderer.h"
#include "Font.h"
#include "Zenith/Renderer/Stats.h"
#include "Zenith/Utils/ZenithException.h"
#include "Zenith/Platform/OpenGL/OpenGLShader.h"
#include "Zenith/Platform/OpenGL/UI/OpenGLUIRenderer.h"

namespace Zenith
{
	extern int MAX_TEXTURES;

	UIRenderer* UIRenderer::Create(Graphics* gfx, Shader* t, Shader* i)
	{
		switch (gfx->GetAPIType())
		{
		case Graphics::API::OpenGL:
			return new OpenGLUIRenderer(gfx, t, i);
		case Graphics::API::D3D11:
			// TODO: Implement for D3D11
			break;
		default:
			// TODO: Throw an exception
			break;
		}
	}

	UIRenderer::UIRenderer(Graphics* gfx, Shader* textShader, Shader* imageShader)
		:
		m_Gfx(gfx),
		m_TextShader(textShader),
		m_ImageShader(imageShader)
	{
	}

	UIRenderer::~UIRenderer()
	{
		DestroyTextRenderer();
		DestroyImageRenderer();
	}

	void UIRenderer::Initialize()
	{
		InitializeTextRenderer(m_TextShader);
		InitializeImageRenderer(m_ImageShader);
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
			offset.x += ch.Advance * fontSize / (float)fontFamily->m_FontSize;
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
			textboxDimensions.x += (ch.Bearing.x + ch.Advance) * scaleFactor;
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

		m_TextIndexCount += 6;

		RendererStats::s_TriangleCount += 2;
		RendererStats::s_VertexCount += 4;
	}

	void UIRenderer::InitializeTextRenderer(Shader* textShader)
	{
		m_TextShader = textShader;
		m_TextQuadBuffer = new Vertex[QUAD_PER_BATCH];
		m_TextTextureSlotIndex = 0;
	}

	void UIRenderer::InitializeImageRenderer(Shader* imageShader)
	{
		m_ImageShader = imageShader;
		m_ImageQuadBuffer = new Vertex[QUAD_PER_BATCH];
		m_ImageTextureSlotIndex = 0;
	}

	void UIRenderer::DestroyTextRenderer()
	{
		
		delete[] m_TextQuadBuffer;
	}
	
	void UIRenderer::DestroyImageRenderer()
	{
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
		m_TextIndexCount = 0;
		m_TextTextureSlotIndex = 0;
		++RendererStats::s_BatchCount;
	}

	void UIRenderer::EndImageRenderer()
	{
		m_ImageIndexCount = 0;
		m_ImageTextureSlotIndex = 0;
		++RendererStats::s_BatchCount;
	}
}
