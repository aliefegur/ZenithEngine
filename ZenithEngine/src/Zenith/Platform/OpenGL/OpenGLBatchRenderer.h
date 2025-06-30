#pragma once

#include <Zenith/Renderer/BatchRenderer.h>

namespace Zenith
{
	class OpenGLBatchRenderer : public BatchRenderer
	{
	public:
		OpenGLBatchRenderer(Graphics* gfx, Shader* shader);
		~OpenGLBatchRenderer();

		void End() override;

		void DrawTexture(Texture2D* texture, const glm::mat4& transform, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }) override;

	private:
		uint32_t	m_VBO = 0,
					m_VAO = 0,
					m_EBO = 0,
					*m_TextureSlots = nullptr;
	};
}
