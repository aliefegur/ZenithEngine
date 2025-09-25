#pragma once

#include <exception>
#include <string>

namespace Zenith
{
	class ZenithException : public std::exception
	{
	public:
		ZenithException(int line, const char* file) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		int GetLine() const noexcept;
		const std::string& GetFile() const noexcept;
		std::string GetOriginString() const noexcept;
	
	private:
		int m_Line;
		std::string m_File;

	protected:
		mutable std::string m_WhatBuffer;
	};

#if ZENITH_PLATFORM_WINDOWS
	class ZenithWException : public std::exception
	{
	public:
		ZenithWException(int line, const wchar_t* file) noexcept;
		const char* what() const noexcept override;
		virtual const wchar_t* what_unicode() const noexcept;
		virtual const wchar_t* GetType() const noexcept;
		int GetLine() const noexcept;
		const std::wstring& GetFile() const noexcept;
		std::wstring GetOriginString() const noexcept;

	private:
		int m_Line;
		std::wstring m_File;

	protected:
		mutable std::wstring m_WhatBuffer;
	};
#endif // ZENITH_PLATFORM_WINDOWS

	class ResourceNotFoundException : public ZenithException
	{
	public:
		ResourceNotFoundException(int line, const char* file, const std::string& path);
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;

		std::string GetNotFoundPath() const noexcept;

	private:
		std::string m_NotFoundPath;
	};

	class InitializationError : public ZenithException
	{
	public:
		InitializationError(int line, const char* file, const std::string& errorDetails) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorDetails() const noexcept;
	private:
		std::string m_Details;
	};
}
