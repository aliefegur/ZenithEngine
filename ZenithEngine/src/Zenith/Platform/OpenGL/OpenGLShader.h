#pragma once

#include "Zenith/Renderer/Shader.h"
#include "Zenith/Utils/ZenithException.h"

#include <cstdint>

#include <glm/glm.hpp>

namespace Zenith
{
	class OpenGLShader : public Shader
	{
	public:
#pragma region Exceptions
		class CompileError : public ZenithException
		{
		public:
			CompileError(int line, const char* file, const std::string& glslFile, const std::string& infoLog);
			const char* what() const noexcept override;
			virtual const char* GetType() const noexcept override;

			std::string GetGLSLSrcFile() const noexcept;
			std::string GetGPULog() const noexcept;

		private:
			std::string m_GlslFile;
			std::string m_InfoLog;
		};

		class LinkError : public ZenithException
		{
		public:
			LinkError(int line, const char* file, uint32_t shaderProgramID, const std::string& infoLog);
			const char* what() const noexcept override;
			virtual const char* GetType() const noexcept override;

			uint32_t GetProgramID() const noexcept;
			std::string GetGPULog() const noexcept;

		private:
			uint32_t m_ProgramID;
			std::string m_InfoLog;
		};
#pragma endregion
	public:
		OpenGLShader(Graphics* gfx, const std::string& vsFile, const std::string& fsFile);
		~OpenGLShader();
		void Bind(Graphics* gfx) noexcept;
		uint32_t GetID() const noexcept;

#pragma region UniformSetters
		void SetUniformInt(const std::string& varName, int value) const noexcept;
		void SetUniformIVec2(const std::string& varName, const glm::ivec2& value) const noexcept;
		void SetUniformIVec3(const std::string& varName, const glm::ivec3& value) const noexcept;
		void SetUniformIVec4(const std::string& varName, const glm::ivec4& value) const noexcept;
		void SetUniformIntArray(const std::string& varName, int* array, int arraySize) const noexcept;

		void SetUniformUInt(const std::string& varName, unsigned int value) const noexcept;
		void SetUniformUVec2(const std::string& varName, const glm::uvec2& value) const noexcept;
		void SetUniformUVec3(const std::string& varName, const glm::uvec3& value) const noexcept;
		void SetUniformUVec4(const std::string& varName, const glm::uvec4& value) const noexcept;
		void SetUniformUIntArray(const std::string& varName, unsigned int* array, int arraySize) const noexcept;

		void SetUniformDouble(const std::string& varName, double value) const noexcept;
		void SetUniformDVec2(const std::string& varName, const glm::dvec2& value) const noexcept;
		void SetUniformDVec3(const std::string& varName, const glm::dvec3& value) const noexcept;
		void SetUniformDVec4(const std::string& varName, const glm::dvec4& value) const noexcept;
		void SetUniformDoubleArray(const std::string& varName, double* array, int arraySize) const noexcept;
		void SetUniformDMatrix2(const std::string& varName, const glm::dmat2& value) const noexcept;
		void SetUniformDMatrix2x3(const std::string& varName, const glm::dmat2x3& value) const noexcept;
		void SetUniformDMatrix2x4(const std::string& varName, const glm::dmat2x4& value) const noexcept;
		void SetUniformDMatrix3x2(const std::string& varName, const glm::dmat3x2& value) const noexcept;
		void SetUniformDMatrix3(const std::string& varName, const glm::dmat3& value) const noexcept;
		void SetUniformDMatrix3x4(const std::string& varName, const glm::dmat3x4& value) const noexcept;
		void SetUniformDMatrix4x2(const std::string& varName, const glm::dmat4x2& value) const noexcept;
		void SetUniformDMatrix4x3(const std::string& varName, const glm::dmat4x3& value) const noexcept;
		void SetUniformDMatrix4(const std::string& varName, const glm::dmat4& value) const noexcept;

		void SetUniformFloat(const std::string& varName, float value) const noexcept;
		void SetUniformVec2(const std::string& varName, const glm::vec2& value) const noexcept;
		void SetUniformVec3(const std::string& varName, const glm::vec3& value) const noexcept;
		void SetUniformVec4(const std::string& varName, const glm::vec4& value) const noexcept;
		void SetUniformFloatArray(const std::string& varName, float* array, int arraySize) const noexcept;
		void SetUniformMatrix2(const std::string& varName, const glm::mat2& value) const noexcept;
		void SetUniformMatrix2x3(const std::string& varName, const glm::mat2x3& value) const noexcept;
		void SetUniformMatrix2x4(const std::string& varName, const glm::mat2x4& value) const noexcept;
		void SetUniformMatrix3x2(const std::string& varName, const glm::mat3x2& value) const noexcept;
		void SetUniformMatrix3(const std::string& varName, const glm::mat3& value) const noexcept;
		void SetUniformMatrix3x4(const std::string& varName, const glm::mat3x4& value) const noexcept;
		void SetUniformMatrix4x2(const std::string& varName, const glm::mat4x2& value) const noexcept;
		void SetUniformMatrix4x3(const std::string& varName, const glm::mat4x3& value) const noexcept;
		void SetUniformMatrix4(const std::string& varName, const glm::mat4& value) const noexcept;
#pragma endregion

	private:
		void AttachShader(const std::string& shaderFile, uint32_t type);
		void LinkProgram();

	private:
		uint32_t m_ProgramID;
	};
}
