#pragma once

#include <assetpackages/AssetPackage.hxx>
#include <assetpackages/IAssetLoader.hxx>

#include <filesystem>
#include <memory>
#include <string>

namespace assetpackages = kyanite::engine::assetpackages;

struct sqlite3;

namespace kyanite::editor {
	enum class AssetType {
		SPRITE,
		MODEL,
		MESH,
		MATERIAL,
		TEXTURE,
		TERRAIN,
		SOUND,
		WORLD,
	};

	class AssetDatabase: public assetpackages::IAssetLoader {
	public:
		AssetDatabase();
		~AssetDatabase();
		auto AddAsset(std::string uuid, std::string path) -> void;
		auto Load(std::filesystem::path path) -> void; 
		auto LoadPackageList(std::string path) -> std::vector<assetpackages::AssetPackage*> override;
		auto CheckIfPackageHasAsset(const assetpackages::AssetPackage* package, std::string path) -> bool override;
		auto LoadPackage(std::string path) -> assetpackages::AssetPackage* override;
		auto LoadAsset(const assetpackages::AssetPackage* package, std::string path) -> std::vector<uint8_t> override;

	private:
		sqlite3* _database;

		auto CreateDatabase(std::filesystem::path path) -> void;
	};
}