#include "zenithpch.h"
#include "UUID.h"

namespace Zenith
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_RandomEngine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_Distribution(0, UINT64_MAX);

	UUID::UUID()
		:
		m_UUID(s_Distribution(s_RandomDevice))
	{
	}

	UUID::UUID(uint64_t id)
		: 
		m_UUID(id)
	{
	}

	UUID::operator uint64_t() const
	{
		return m_UUID;
	}
}
