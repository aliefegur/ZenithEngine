#pragma once

#include "Asset.h"

namespace Zenith
{
	class AssetManager
	{
		friend class ZenithApp;

	private:
		AssetManager();

	public:
		Asset* Load(const UUID& assetUUID);
		void Unload(const UUID& assetUUID);

	private:
		std::unordered_map<uint64_t, std::shared_ptr<Asset>> m_Assets;
	};
}
