#include "zenithpch.h"
#include "OpenGLShader.h"
#include "Zenith/Utils/File.h"

#define PARAMETER_UNIFORM_1N glGetUniformLocation(m_ProgramID, varName.c_str()), value
#define PARAMETER_UNIFORM_2N glGetUniformLocation(m_ProgramID, varName.c_str()), value.x, value.y
#define PARAMETER_UNIFORM_3N PARAMETER_UNIFORM_2N, value.z
#define PARAMETER_UNIFORM_4N PARAMETER_UNIFORM_3N, value.w
#define PARAMETER_UNIFORM_ARRAY glGetUniformLocation(m_ProgramID, varName.c_str()), arraySize, array
#define PARAMETER_UNIFORM_MATRIX glGetUniformLocation(m_ProgramID, varName.c_str()), 1, GL_FALSE, glm::value_ptr(value)

namespace Zenith
{
	static std::string PreprocessGLSL_Source(const std::string& src)
	{
		int maxTextures;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);
		return std::regex_replace(src, std::regex("#define MAX_TEXTURES"), "#define MAX_TEXTURES " + std::to_string(maxTextures));
	}

	OpenGLShader::OpenGLShader(Graphics* gfx, const std::string& vsFile, const std::string& fsFile)
		:
		Shader(gfx, vsFile, fsFile)
	{
		m_ProgramID = glCreateProgram();
		AttachShader(vsFile, GL_VERTEX_SHADER);
		AttachShader(fsFile, GL_FRAGMENT_SHADER);
		LinkProgram();
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteShader(m_ProgramID);
	}
	void OpenGLShader::Bind() const noexcept
	{
		glUseProgram(m_ProgramID);
	}
	uint32_t OpenGLShader::GetID() const noexcept
	{
		return m_ProgramID;
	}

	void OpenGLShader::AttachShader(const std::string& shaderFile, uint32_t type)
	{
		// Read shader source
		std::string _src = PreprocessGLSL_Source(File::Read(shaderFile));
		const char* src = _src.c_str();

		// Create & compile shader
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);

		// Compile errors
		int isCompiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (!isCompiled)
		{
			char log[1024];
			glGetShaderInfoLog(shader, 1024, nullptr, log);
			throw CompileError(__LINE__, __FILE__, shaderFile, log);
		}

		// Attach compiled shader
		glAttachShader(m_ProgramID, shader);

		// Cleanup
		glDeleteShader(shader);
	}

	void OpenGLShader::LinkProgram()
	{
		glLinkProgram(m_ProgramID);

		// Linkage errors
		int isLinked;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &isLinked);
		if (!isLinked)
		{
			char log[1024];
			glGetProgramInfoLog(m_ProgramID, 1024, nullptr, log);
			throw LinkError(__LINE__, __FILE__, m_ProgramID, log);
		}
	}

#pragma region Uniform Setters

#pragma region Integer
	void OpenGLShader::SetUniformInt(const std::string& varName, int value) const noexcept
	{
		glUniform1i(PARAMETER_UNIFORM_1N);
	}
	void OpenGLShader::SetUniformIVec2(const std::string& varName, const glm::ivec2& value) const noexcept
	{
		glUniform2i(PARAMETER_UNIFORM_2N);
	}
	void OpenGLShader::SetUniformIVec3(const std::string& varName, const glm::ivec3& value) const noexcept
	{
		glUniform3i(PARAMETER_UNIFORM_3N);
	}
	void OpenGLShader::SetUniformIVec4(const std::string& varName, const glm::ivec4& value) const noexcept
	{
		glUniform4i(PARAMETER_UNIFORM_4N);
	}
	void OpenGLShader::SetUniformIntArray(const std::string& varName, int* array, int arraySize) const noexcept
	{
		glUniform1iv(PARAMETER_UNIFORM_ARRAY);
	}
#pragma endregion
#pragma region Unsigned Integer
	void OpenGLShader::SetUniformUInt(const std::string& varName, unsigned int value) const noexcept
	{
		glUniform1ui(PARAMETER_UNIFORM_1N);
	}
	void OpenGLShader::SetUniformUVec2(const std::string& varName, const glm::uvec2& value) const noexcept
	{
		glUniform2ui(PARAMETER_UNIFORM_2N);
	}
	void OpenGLShader::SetUniformUVec3(const std::string& varName, const glm::uvec3& value) const noexcept
	{
		glUniform3ui(PARAMETER_UNIFORM_3N);
	}
	void OpenGLShader::SetUniformUVec4(const std::string& varName, const glm::uvec4& value) const noexcept
	{
		glUniform4ui(PARAMETER_UNIFORM_4N);
	}
	void OpenGLShader::SetUniformUIntArray(const std::string& varName, unsigned int* array, int arraySize) const noexcept
	{
		glUniform1uiv(PARAMETER_UNIFORM_ARRAY);
	}
#pragma endregion
#pragma region Double
	void OpenGLShader::SetUniformDouble(const std::string& varName, double value) const noexcept
	{
		glUniform1d(PARAMETER_UNIFORM_1N);
	}
	void OpenGLShader::SetUniformDVec2(const std::string& varName, const glm::dvec2& value) const noexcept
	{
		glUniform2d(PARAMETER_UNIFORM_2N);
	}
	void OpenGLShader::SetUniformDVec3(const std::string& varName, const glm::dvec3& value) const noexcept
	{
		glUniform3d(PARAMETER_UNIFORM_3N);
	}
	void OpenGLShader::SetUniformDVec4(const std::string& varName, const glm::dvec4& value) const noexcept
	{
		glUniform4d(PARAMETER_UNIFORM_4N);
	}
	void OpenGLShader::SetUniformDoubleArray(const std::string& varName, double* array, int arraySize) const noexcept
	{
		glUniform1dv(PARAMETER_UNIFORM_ARRAY);
	}
	void OpenGLShader::SetUniformDMatrix2(const std::string& varName, const glm::dmat2& value) const noexcept
	{
		glUniformMatrix2dv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformDMatrix2x3(const std::string& varName, const glm::dmat2x3& value) const noexcept
	{
		glUniformMatrix2x3dv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformDMatrix2x4(const std::string& varName, const glm::dmat2x4& value) const noexcept
	{
		glUniformMatrix2x4dv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformDMatrix3x2(const std::string& varName, const glm::dmat3x2& value) const noexcept
	{
		glUniformMatrix3x2dv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformDMatrix3(const std::string& varName, const glm::dmat3& value) const noexcept
	{
		glUniformMatrix3dv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformDMatrix3x4(const std::string& varName, const glm::dmat3x4& value) const noexcept
	{
		glUniformMatrix3x4dv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformDMatrix4x2(const std::string& varName, const glm::dmat4x2& value) const noexcept
	{
		glUniformMatrix4x2dv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformDMatrix4x3(const std::string& varName, const glm::dmat4x3& value) const noexcept
	{
		glUniformMatrix4x3dv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformDMatrix4(const std::string& varName, const glm::dmat4& value) const noexcept
	{
		glUniformMatrix4dv(PARAMETER_UNIFORM_MATRIX);
	}
#pragma endregion
#pragma region Float
	void OpenGLShader::SetUniformFloat(const std::string& varName, float value) const noexcept
	{
		glUniform1f(PARAMETER_UNIFORM_1N);
	}
	void OpenGLShader::SetUniformVec2(const std::string& varName, const glm::vec2& value) const noexcept
	{
		glUniform2f(PARAMETER_UNIFORM_2N);
	}
	void OpenGLShader::SetUniformVec3(const std::string& varName, const glm::vec3& value) const noexcept
	{
		glUniform3f(PARAMETER_UNIFORM_3N);
	}
	void OpenGLShader::SetUniformVec4(const std::string& varName, const glm::vec4& value) const noexcept
	{
		glUniform4f(PARAMETER_UNIFORM_4N);
	}
	void OpenGLShader::SetUniformFloatArray(const std::string& varName, float* array, int arraySize) const noexcept
	{
		glUniform1fv(PARAMETER_UNIFORM_ARRAY);
	}
	void OpenGLShader::SetUniformMatrix2(const std::string& varName, const glm::mat2& value) const noexcept
	{
		glUniformMatrix2fv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformMatrix2x3(const std::string& varName, const glm::mat2x3& value) const noexcept
	{
		glUniformMatrix2x3fv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformMatrix2x4(const std::string& varName, const glm::mat2x4& value) const noexcept
	{
		glUniformMatrix2x4fv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformMatrix3x2(const std::string& varName, const glm::mat3x2& value) const noexcept
	{
		glUniformMatrix3x2fv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformMatrix3(const std::string& varName, const glm::mat3& value) const noexcept
	{
		glUniformMatrix3fv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformMatrix3x4(const std::string& varName, const glm::mat3x4& value) const noexcept
	{
		glUniformMatrix3x4fv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformMatrix4x2(const std::string& varName, const glm::mat4x2& value) const noexcept
	{
		glUniformMatrix4x2fv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformMatrix4x3(const std::string& varName, const glm::mat4x3& value) const noexcept
	{
		glUniformMatrix4x3fv(PARAMETER_UNIFORM_MATRIX);
	}
	void OpenGLShader::SetUniformMatrix4(const std::string& varName, const glm::mat4& value) const noexcept
	{
		glUniformMatrix4fv(PARAMETER_UNIFORM_MATRIX);
	}
#pragma endregion

#pragma endregion

#pragma region Exceptions
	OpenGLShader::CompileError::CompileError(int line, const char* file, const std::string& glslFile, const std::string& infoLog)
		:
		ZenithException(line, file), m_GlslFile(glslFile), m_InfoLog(infoLog)
	{
	}

	const char* OpenGLShader::CompileError::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[GLSL Source File] " << m_GlslFile << std::endl
			<< "\n[GPU Log] " << std::endl << m_InfoLog << std::endl
			<< "\n" << GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* OpenGLShader::CompileError::GetType() const noexcept
	{
		return "GLSL Shader Compilation Error";
	}
	std::string OpenGLShader::CompileError::GetGLSLSrcFile() const noexcept
	{
		return m_GlslFile;
	}
	std::string OpenGLShader::CompileError::GetGPULog() const noexcept
	{
		return m_InfoLog;
	}

	OpenGLShader::LinkError::LinkError(int line, const char* file, uint32_t shaderProgramID, const std::string& infoLog)
		:
		ZenithException(line, file), m_ProgramID(shaderProgramID), m_InfoLog(infoLog)
	{
	}
	const char* OpenGLShader::LinkError::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Program ID] 0x" << std::hex << std::uppercase << m_ProgramID << std::endl
			<< "\n[GPU Log]" << std::endl << m_InfoLog << std::endl
			<< "\n" << GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* OpenGLShader::LinkError::GetType() const noexcept
	{
		return "OpenGL Shader Pipeline Linkage Error";
	}
	uint32_t OpenGLShader::LinkError::GetProgramID() const noexcept
	{
		return m_ProgramID;
	}
	std::string OpenGLShader::LinkError::GetGPULog() const noexcept
	{
		return m_InfoLog;
	}
}
#pragma endregion
