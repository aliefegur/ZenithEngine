#include "zenithpch.h"
#include "Asset.h"

namespace Zenith
{
#pragma region Asset Base Class
	Asset::Asset()
		:
		m_UUID(UUID())
	{
	}

	Asset::operator UUID() const noexcept
	{
		return m_UUID;
	}

	Asset::operator uint64_t() const noexcept
	{
		return m_UUID;
	}
#pragma endregion

#pragma region Texture2D Asset
	Texture2DAsset::Texture2DAsset(Graphics* gfx, const std::string& imageFile, unsigned int pixelPerUnit, bool forceRGBA, Texture2D::Filter filter, Texture2D::Wrap wrap)
		:
		Asset()
	{
		m_Type = Type::Texture2D;
		m_Texture = Texture2D::LoadFromFile(gfx, imageFile, pixelPerUnit, forceRGBA, filter, wrap);
	}
	
	Texture2DAsset::Texture2DAsset(Graphics* gfx, unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit, Texture2D::Filter filter, Texture2D::Wrap wrap)
		:
		Asset()
	{
		m_Type = Type::Texture2D;
		m_Texture = Texture2D::LoadFromMemory(gfx, pixels, width, height, channelCount, pixelPerUnit, filter, wrap);
	}
	
	Texture2DAsset::~Texture2DAsset()
	{
		delete m_Texture;
	}
	
	Texture2DAsset::operator Texture2D* () const noexcept
	{
		return m_Texture;
	}
#pragma endregion
}
