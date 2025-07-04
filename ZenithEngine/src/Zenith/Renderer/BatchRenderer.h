#pragma once

#include "Texture2D.h"
#include "Shader.h"
#include "Vertex.h"

#include <glm/glm.hpp>

namespace Zenith
{
	class BatchRenderer
	{
	protected:
		BatchRenderer(Graphics* gfx, Shader* shader);

	public:
		static BatchRenderer* Create(Graphics* gfx, Shader* shader);
		virtual ~BatchRenderer();

		void Begin(const glm::mat4& viewProjection = glm::mat4(1.0f));
		virtual void End();

		virtual void DrawTexture(Texture2D* texture, const glm::mat4& transform, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawTexture(Texture2D* texture, const glm::vec3& pos, const glm::vec3& scale, float zRotation, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawTexture(Texture2D* texture, const glm::vec3& pos, const glm::vec3& scale, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawTexture(Texture2D* texture, const glm::vec3& pos, float rotation, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawTexture(Texture2D* texture, const glm::vec3& pos, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	protected:
		uint32_t	m_IndexCount = 0;
		Vertex		*m_QuadBuffer = nullptr,
					*m_QuadbufferPtr = nullptr;
		uint32_t	m_TextureSlotIndex = 0;
		Shader*		m_ShaderProgram = nullptr;
		Graphics*	m_Gfx = nullptr;
		glm::mat4	m_ViewProj{ 1.0f };
		static constexpr size_t	QUAD_PER_BATCH = 1000;
		static constexpr size_t	MAX_TEXTURES = 16;
	};
}
