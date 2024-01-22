#pragma once

#include <string>
#include <vector>

namespace kyanite::engine::assetpackages {
	class AssetPackage;

	class IAssetLoader {
	public:
		virtual auto LoadPackageList(std::string path) -> std::vector<AssetPackage*> = 0;
		virtual auto CheckIfPackageHasAsset(const AssetPackage* package, std::string path) -> bool = 0;
		virtual auto LoadPackage(std::string path) -> AssetPackage* = 0;
		virtual auto LoadAsset(const AssetPackage* package, std::string path) -> std::vector<uint8_t> = 0;
	};
}