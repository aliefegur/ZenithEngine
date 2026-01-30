#include "zenithpch.h"
#include "FontLoader.h"
#include "Zenith/Utils/ZenithException.h"

#if ZENITH_PLATFORM_WINDOWS
#include <ft2build.h>
#include FT_FREETYPE_H
#else
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#endif

namespace Zenith
{
	static FT_Library s_Ft;
	static bool s_FtInitialized = false;
	static unsigned int s_FontInstanceCount = 0;

	static std::unordered_map<uint64_t, FontLoader::Face> s_Faces;

	static uint64_t HashFaceKey(const std::string& path, unsigned int fontSize)
	{
		std::hash<std::string> strHash;
		std::hash<unsigned int> uintHash;
		
		size_t h1 = strHash(path);
		size_t h2 = uintHash(fontSize);
		
		return h1 ^ (h2 << 1);
	}

	void FontLoader::Init()
	{
		if (!s_FtInitialized)
		{
			if (FT_Init_FreeType(&s_Ft))
			{
				throw InitializationError(__LINE__, __FILE__, "Failed to initialize FreeType library!");
			}
		}
	}

	void FontLoader::Shutdown()
	{
		ClearCache();

		// Shutdown FreeType
		if (s_FtInitialized)
		{
			FT_Done_FreeType(s_Ft);
			s_Ft = nullptr;
			s_FtInitialized = false;
		}
	}

	FontLoader::Face& FontLoader::LoadFace(const std::string& path, unsigned int fontSize)
	{
		size_t key = HashFaceKey(path, fontSize);

		// Cache hit
		auto it = s_Faces.find(key);
		if (it != s_Faces.end())
		{
			return it->second;
		}

		// Create new face
		Face face{};
		face.faceID = key;

		// Create new FreeType face
		FT_Face fontFace;
		if (FT_New_Face(s_Ft, path.c_str(), 0, &fontFace))
			throw ResourceNotFoundException(__LINE__, __FILE__, path);

		// Set pixel size
		FT_Set_Pixel_Sizes(fontFace, 0, fontSize);

		// Load glyphs
		for (uint8_t i = 0; i < 255; ++i)
		{
			if (FT_Load_Char(fontFace, i, FT_LOAD_RENDER))
				continue;

			// Copy FrteeType glyph data to our glyph structure
			auto& glyph = face.glyphs[i];

			glyph.bitmap.width = fontFace->glyph->bitmap.width;
			glyph.bitmap.height = fontFace->glyph->bitmap.rows;

			size_t size = glyph.bitmap.width * glyph.bitmap.height;
			glyph.bitmap.buffer = new unsigned char[size];
			memcpy(glyph.bitmap.buffer, fontFace->glyph->bitmap.buffer, size); // Copy bitmap buffer

			glyph.advance.x = fontFace->glyph->advance.x;
			glyph.advance.y = fontFace->glyph->advance.y;

			glyph.bitmapLeft = fontFace->glyph->bitmap_left;
			glyph.bitmapTop = fontFace->glyph->bitmap_top;
		}

		FT_Done_Face(fontFace);
		
		auto result = s_Faces.emplace(key, std::move(face));
		return result.first->second;
	}

	void FontLoader::UnloadFace(uint64_t face)
	{
		// Find the face from cache
		auto it = s_Faces.find(face);
		if (it == s_Faces.end())
			return;

		// Delete glyph bitmaps (to prevent memory leaks)
		auto& loadedFace = it->second;
		for (auto& glyph : loadedFace.glyphs)
		{
			delete[] glyph.bitmap.buffer;
			glyph.bitmap.buffer = nullptr;
		}

		// Erase face
		s_Faces.erase(it);
	}

	void FontLoader::ClearCache()
	{
		// Delete glyph bitmaps
		for (auto& [id, face] : s_Faces)
		{
			for (auto& glyph : face.glyphs)
			{
				delete[] glyph.bitmap.buffer;
				glyph.bitmap.buffer = nullptr;
			}
		}

		// Clear faces map
		s_Faces.clear();
	}
}
