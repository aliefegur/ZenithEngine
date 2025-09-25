#if ZENITH_PLATFORM_WINDOWS

#pragma once

#include <dxgidebug.h>
#include <vector>
#include <string>

namespace Zenith
{
	class DxgiInfoManager
	{
	public:
		DxgiInfoManager();
		DxgiInfoManager(const DxgiInfoManager&) = delete;
		DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
		~DxgiInfoManager();
		
		void Set();
		std::vector<std::string> GetMessages() const;

	private:
		unsigned long long next = 0u;
		IDXGIInfoQueue* pDxgiInfoQueue = nullptr;
	};
}

#endif // ZENITH_PLATFORM_WINDOWS
