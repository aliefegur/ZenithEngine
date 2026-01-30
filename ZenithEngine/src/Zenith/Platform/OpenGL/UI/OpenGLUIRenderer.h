#pragma once

#include "Zenith/Renderer/UI/UIRenderer.h"

namespace Zenith
{
	class OpenGLUIRenderer : public UIRenderer
	{
	public:
		OpenGLUIRenderer(Graphics* gfx, Shader* textShader, Shader* imageShader);

		void DrawImage(Texture2D* img, const glm::vec2& position, const glm::vec2& scale, float rotation = 0.0f, Anchor anchor = Anchor::MiddleCenter, Anchor pivot = Anchor::MiddleCenter, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }) override;
		void DrawChar(Font* fontFamily, char chr, const glm::vec2& position, float fontSize = 12.0f, const glm::vec4& color = { 0.05f, 0.05f, 0.05f, 1.0f }) override;

	private:
		void InitializeTextRenderer(Shader* textShader) override;
		void InitializeImageRenderer(Shader* imageShader) override;
		void DestroyTextRenderer() override;
		void DestroyImageRenderer() override;

		void EndTextRenderer() override;
		void EndImageRenderer() override;

	private:
		uint32_t	m_TextVBO				= 0,
					m_TextVAO				= 0,
					m_TextEBO				= 0,
					m_ImageVBO				= 0,
					m_ImageVAO				= 0,
					m_ImageEBO				= 0,
					*m_TextTextureSlots		= nullptr,
					*m_ImageTextureSlots	= nullptr;
	};
}
