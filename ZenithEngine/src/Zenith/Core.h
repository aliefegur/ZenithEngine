#pragma once

#include <iostream>
#include <cstdlib>

#ifdef ZN_DEBUG
	#define ZN_ENABLE_ASSERTS
#endif

#ifdef ZN_ENABLE_ASSERTS
	#define ZN_CORE_ASSERT(x, ...) \
		{ if (!(x)) { \
			std::cerr << "[Zenith Engine Assert] " << __VA_ARGS__ << std::endl; \
			std::abort(); \
		} }
#else
	#define ZN_CORE_ASSERT(x, ...)
#endif
