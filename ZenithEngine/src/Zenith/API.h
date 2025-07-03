#pragma once

#ifdef _DEBUG
	#define ZENITH_DEBUG true
#else
	#define ZENITH_DEBUG false
#endif

#ifdef _WIN32
	#define ZENITH_PLATFORM_WINDOWS
	#ifdef ZENITH_SHARED
		#ifdef ZENITH_BUILD_DLL
			#define ZENITH_API __declspec(dllexport)
		#else
			#define ZENITH_API __declspec(dllimport)
		#endif
	#else
		#define ZENITH_API
	#endif
#else
	#error "Zenith Engine currently only supports Windows platform."
#endif
