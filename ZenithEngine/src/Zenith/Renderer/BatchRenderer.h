#pragma once

#include "Texture2D.h"
#include "Shader.h"
#include "Vertex.h"

#include <glm/glm.hpp>

namespace Zenith
{
	class BatchRenderer
	{
	public:
		BatchRenderer(Graphics* gfx, Shader* shader);
		~BatchRenderer();

		void Begin(const glm::mat4& viewProjection = glm::mat4(1.0f));
		void End();

		void DrawTexture(Texture2D* texture, const glm::mat4& transform, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawTexture(Texture2D* texture, const glm::vec3& pos, const glm::vec3& scale, float zRotation, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawTexture(Texture2D* texture, const glm::vec3& pos, const glm::vec3& scale, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawTexture(Texture2D* texture, const glm::vec3& pos, float rotation, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawTexture(Texture2D* texture, const glm::vec3& pos, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	private:
		uint32_t	m_VBO = 0,
					m_VAO = 0,
					m_EBO = 0,
					m_IndexCount = 0;
		Vertex		*m_QuadBuffer = nullptr,
					*m_QuadbufferPtr = nullptr;
		uint32_t	*m_TextureSlots = nullptr,
					m_TextureSlotIndex = 0;
		Shader*		m_ShaderProgram = nullptr;
		Graphics*	m_Gfx = nullptr;
		glm::mat4	m_ViewProj{ 1.0f };
	};
}
