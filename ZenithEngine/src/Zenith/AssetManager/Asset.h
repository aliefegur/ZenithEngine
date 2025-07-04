#pragma once

#include "Zenith/API.h"
#include "Zenith/Utils/UUID.h"
#include "Zenith/Renderer/Texture2D.h"

namespace Zenith
{
	class ZENITH_API Asset
	{
		friend class AssetManager;
	public:
		enum class Type
		{
			Texture2D = 0x1,
			Shader = 0x2,
			Font = 0x3,
			Audio = 0x4,
			Unknown = NULL
		};

	protected:
		Asset();

	public:
		virtual ~Asset() = default;
		operator UUID() const noexcept;
		operator uint64_t() const noexcept;

	protected:
		UUID m_UUID;
		Type m_Type = Type::Unknown;
	};

	class ZENITH_API Texture2DAsset : public Asset
	{
		friend class AssetManager;
	protected:
		Texture2DAsset(Graphics* gfx, const std::string& imageFile, unsigned int pixelPerUnit = 100u, bool forceRGBA = false, Texture2D::Filter filter = Texture2D::Filter::Bilinear, Texture2D::Wrap wrap = Texture2D::Wrap::Clamp);
		Texture2DAsset(Graphics* gfx, unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit = 100u, Texture2D::Filter filter = Texture2D::Filter::Bilinear, Texture2D::Wrap wrap = Texture2D::Wrap::Clamp);
		~Texture2DAsset() override;

	public:
		operator Texture2D* () const noexcept;

	private:
		Texture2D* m_Texture;
	};
}
