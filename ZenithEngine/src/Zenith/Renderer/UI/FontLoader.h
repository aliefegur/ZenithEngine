#pragma once

#include <cstdint>
#include <string>

namespace Zenith
{
	class FontLoader
	{
	public:
		struct Face
		{
			struct Glyph
			{
				struct Bitmap
				{
					uint32_t	width;
					uint32_t	height;
					uint8_t*	buffer;
				};

				struct Advance
				{
					int32_t x;
					int32_t y;
				};

				Bitmap	bitmap;
				Advance	advance;
				int32_t	bitmapLeft;
				int32_t	bitmapTop;
			};

			Glyph glyphs[256];
			uint64_t faceID;

			operator uint64_t() const { return faceID; }
		};

	public:
		static void Init();
		static void Shutdown();
		static Face& LoadFace(const std::string& path, unsigned int fontSize);
		static void UnloadFace(uint64_t face);
		static void ClearCache();
	};
}
