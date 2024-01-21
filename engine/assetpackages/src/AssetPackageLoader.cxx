#include "assetpackages/AssetPackageLoader.hxx"
#include "assetpackages/AssetPackage.hxx"
#include <core/Core.hxx>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/json.hpp>
#include <zip.h>

#include <algorithm>
#include <filesystem>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace kyanite::engine::assetpackages {
	AssetPackageLoader::AssetPackageLoader() {

	}

	AssetPackageLoader::~AssetPackageLoader() {

	}

	auto AssetPackageLoader::LoadPackageList(std::string path) -> std::vector<AssetPackage*> {
		// Get all the files in the directory that end with .kpack and create a vector of AssetPackage pointers from them
		std::vector<AssetPackage*> packages;
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			if (entry.path().extension() == ".kpack") {
				packages.push_back(LoadPackage(entry.path().string()));
			}
		}

		return packages;
	}

	auto AssetPackageLoader::CheckIfPackageHasAsset(const AssetPackage* package, std::string path) -> bool {
		return core::CheckIfFileExists(package->_path, path);
	}

	auto AssetPackageLoader::LoadPackage(std::string path) -> AssetPackage* {
		try {
			core::OpenArchive(path);
		} catch (std::exception& e) {
			return nullptr;
		}

		auto ptr = new AssetPackage(path);

		return ptr;
	}

	auto AssetPackageLoader::LoadAsset(const AssetPackage* package, std::string path) -> std::vector<uint8_t> {
		if(core::CheckIfFileExists(package->_path, path)) {
			return core::LoadFileFromArchive(package->_path, path);
		} else {
			throw std::runtime_error("Asset does not exist in package");
		}
	}
}