#pragma once

#include "Bindable.h"
#include <string>

namespace Zenith
{
	class Shader : public virtual Bindable
	{
	public:
		Shader(Graphics* gfx, const std::string& vsFile, const std::string& psFile);
		~Shader() = default;

		virtual void Bind(Graphics* gfx) noexcept override = 0;
	};
}
