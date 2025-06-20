#include "zenithpch.h"
#include "ZenithUtils.h"

namespace Zenith
{
	std::wstring Utils::ToWide(const std::string& input)
	{
		std::wstring out;

		size_t requiredSize = std::mbstowcs(nullptr, input.c_str(), 0);
		if (requiredSize == -1) return nullptr;

		out.resize(requiredSize);
		std::mbstowcs(&out[0], input.c_str(), requiredSize);

		return out;
	}
}
