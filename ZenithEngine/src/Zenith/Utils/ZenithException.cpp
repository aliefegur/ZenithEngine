#include "zenithpch.h"
#include "ZenithException.h"

namespace Zenith
{
#pragma region ZenithException

	ZenithException::ZenithException(int line, const char* file) noexcept
		: m_Line(line), m_File(file)
	{
	}
	const char* ZenithException::what() const noexcept
	{
		std::ostringstream oss;

		oss << GetType() << std::endl
			<< GetOriginString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* ZenithException::GetType() const noexcept
	{
		return "Engine Exception";
	}
	int ZenithException::GetLine() const noexcept
	{
		return m_Line;
	}
	const std::string& ZenithException::GetFile() const noexcept
	{
		return m_File;
	}
	std::string ZenithException::GetOriginString() const noexcept
	{
		std::ostringstream oss;
		oss << "[File] " << m_File << std::endl
			<< "[Line] " << m_Line;
		return oss.str();
	}
	
#pragma endregion

#pragma region ZenithWException
#if ZENITH_PLATFORM_WINDOWS

	ZenithWException::ZenithWException(int line, const wchar_t* file) noexcept
		:
		m_Line(line), m_File(file)
	{
	}
	const char* ZenithWException::what() const noexcept
	{
		const wchar_t* unicode_what = what_unicode();
		size_t outputSize = m_WhatBuffer.size() + 1;
		char* output = new char[outputSize];
		wcstombs_s(nullptr, output, outputSize, unicode_what, _TRUNCATE);
		return output;
	}
	const wchar_t* ZenithWException::what_unicode() const noexcept
	{
		std::wostringstream wos;
		wos << GetType() << std::endl
			<< GetOriginString();
		m_WhatBuffer = wos.str();
		return m_WhatBuffer.c_str();
	}
	const wchar_t* ZenithWException::GetType() const noexcept
	{
		return L"Engine Exception";
	}
	int ZenithWException::GetLine() const noexcept
	{
		return m_Line;
	}
	const std::wstring& ZenithWException::GetFile() const noexcept
	{
		return m_File;
	}
	std::wstring ZenithWException::GetOriginString() const noexcept
	{
		std::wostringstream wos;
		wos << L"[File] " << m_File << std::endl
			<< L"[Line] " << m_Line;
		return wos.str();
	}

#endif // ZENITH_PLATFORM_WINDOWS
#pragma endregion

#pragma region ResourceNotFoundException

	ResourceNotFoundException::ResourceNotFoundException(int line, const char* file, const std::string& path)
		:
		ZenithException(line, file), m_NotFoundPath(path)
	{
	}
	const char* ResourceNotFoundException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Missing File] " << GetNotFoundPath() << std::endl
			<< GetOriginString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* ResourceNotFoundException::GetType() const noexcept
	{
		return "Resource Not Found";
	}
	std::string ResourceNotFoundException::GetNotFoundPath() const noexcept
	{
		return m_NotFoundPath;
	}

#pragma endregion

#pragma region InitializationError

	InitializationError::InitializationError(int line, const char* file, const std::string& errorDetails) noexcept
		:
		ZenithException(line, file),
		m_Details(errorDetails)
	{
	}
	const char* InitializationError::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Details] " << GetErrorDetails() << std::endl
			<< GetOriginString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* InitializationError::GetType() const noexcept
	{
		return "Initialization Error";
	}
	std::string InitializationError::GetErrorDetails() const noexcept
	{
		return m_Details;
	}

#pragma endregion
}
