#include "assetpackages/AssetPackageBuilder.hxx"
#include "assetpackages/AssetPackage.hxx"

#include <core/Core.hxx>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/external/rapidjson/rapidjson.h>
#include <nlohmann/json.hpp>

#include <filesystem>
#include <sstream>
#include <vector>

namespace core = kyanite::engine::core;

namespace kyanite::engine::assetpackages {
	AssetPackageBuilder::AssetPackageBuilder() {

	}

	AssetPackageBuilder::~AssetPackageBuilder() {

	}

	auto AssetPackageBuilder::Create(std::string path) -> AssetPackage* {
		auto package = new AssetPackage(path);

		core::CreateArchive(path);

		return package;
	}

	auto AssetPackageBuilder::AddAsset(AssetPackage* package, std::string path, std::string data) -> void {
		core::SaveBufferToArchive(package->_path, path, data);
	}
}