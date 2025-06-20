#pragma once

#include "Graphics.h"

namespace Zenith
{
	class Bindable
	{
	public:
		virtual void Bind(Graphics* gfx) noexcept = 0;
		virtual ~Bindable() = default;
	};
}
