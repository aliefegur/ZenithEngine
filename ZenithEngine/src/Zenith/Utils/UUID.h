#pragma once

#include "Zenith/API.h"

namespace Zenith
{
	class ZENITH_API UUID
	{
	public:
		UUID();
		UUID(uint64_t id);
		UUID(const UUID& other) = default;
		UUID& operator=(const UUID& other) = default;
		operator uint64_t() const;
	private:
		uint64_t m_UUID;
	};
}
