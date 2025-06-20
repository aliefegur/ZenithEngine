#pragma once

#include "Font.h"
#include "../Shader.h"
#include "../Vertex.h"

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace Zenith
{
	enum class Anchor
	{
		TopLeft = 0, TopCenter = 1, TopRight = 2,
		MiddleLeft = 3, MiddleCenter = 4, MiddleRight = 5,
		BottomLeft = 6, BottomCenter = 7, BottomRight = 8
	};

	class UIRenderer
	{
	public:
		UIRenderer(Graphics* gfx, Shader* textShader, Shader* imageShader);
		~UIRenderer();

		void Begin(const glm::ivec2& windowDimensions);
		void End();

		void DrawImage(Texture2D* img, const glm::vec2& position, const glm::vec2& scale, float rotation = 0.0f, Anchor anchor = Anchor::MiddleCenter, Anchor pivot = Anchor::MiddleCenter, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawTxt(Font* fontFamily, const std::string& text, const glm::vec2& position, float fontSize = 12.0f, const glm::vec4& color = { 0.05f, 0.05f, 0.05f, 1.0f });
		void DrawTxt(Font* fontFamily, const std::string& text, const glm::vec2& position, Anchor anchor, float fontSize = 12.0f, const glm::vec4& color = { 0.05f, 0.05f, 0.05f, 1.0f });
		void DrawChar(Font* fontFamily, char chr, const glm::vec2& position, float fontSize = 12.0f, const glm::vec4& color = { 0.05f, 0.05f, 0.05f, 1.0f });

	private:
		void InitializeTextRenderer(Shader* textShader);
		void InitializeImageRenderer(Shader* imageShader);
		void DestroyTextRenderer();
		void DestroyImageRenderer();

		void BeginTextRenderer();
		void BeginImageRenderer();
		void EndTextRenderer();
		void EndImageRenderer();

	private:
		uint32_t	m_TextVBO				= 0,
					m_TextVAO				= 0,
					m_TextEBO				= 0,
					m_TextIndexCount		= 0,
					m_ImageVBO				= 0,
					m_ImageVAO				= 0,
					m_ImageEBO				= 0,
					m_ImageIndexCount		= 0;
		Vertex		*m_TextQuadBuffer		= nullptr,
					*m_TextQuadBufferPtr	= nullptr,
					*m_ImageQuadBuffer		= nullptr,
					*m_ImageQuadBufferPtr	= nullptr;
		uint32_t	*m_TextTextureSlots		= nullptr,
					m_TextTextureSlotIndex	= 0,
					*m_ImageTextureSlots	= nullptr,
					m_ImageTextureSlotIndex	= 0;
		Shader		*m_TextShader			= nullptr,
					*m_ImageShader			= nullptr;
		Graphics*	m_Gfx					= nullptr;
		glm::ivec2	m_WindowDimensions		= glm::ivec2{ 0 };
	};
}
