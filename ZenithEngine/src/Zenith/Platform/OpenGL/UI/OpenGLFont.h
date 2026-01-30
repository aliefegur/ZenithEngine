#pragma once

#include "Zenith/Renderer/UI/Font.h"

namespace Zenith
{
	class OpenGLFont : public Font
	{
		friend class OpenGLUIRenderer;
	public:
		OpenGLFont(const std::string& path, unsigned int fontSize, Texture2D::Filter filterMode = Texture2D::Filter::Bilinear);
		~OpenGLFont();

	private:
		std::map<unsigned char, uint32_t> m_TextureIDs;
	};
}
