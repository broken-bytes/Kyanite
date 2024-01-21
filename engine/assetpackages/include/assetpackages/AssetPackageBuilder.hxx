#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace kyanite::engine::assetpackages {
	class AssetPackage;

	class AssetPackageBuilder {
	public:
		AssetPackageBuilder();
		~AssetPackageBuilder();

		auto Create(std::string path) -> AssetPackage*;
		auto AddAsset(AssetPackage* package, std::string path, std::string data) -> void;
	};
}