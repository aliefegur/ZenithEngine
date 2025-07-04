#pragma once

#include "Bindable.h"
#include <string>

namespace Zenith
{
	class Shader : public virtual Bindable
	{
	public:
		static Shader* LoadShader(Graphics* gfx, const std::string& vsFile, const std::string& psFile);

	protected:
		Shader(Graphics* gfx, const std::string& vsFile, const std::string& psFile);

	public:
		~Shader() = default;

		virtual void Bind(Graphics* gfx) noexcept override = 0;
	};
}
