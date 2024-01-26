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
		ANIMATION,
		MATERIAL,
		MESH,
		MODEL,
		SHADER,
		SOUND,
		SPRITE,
		TERRAIN,
		TEXTURE,
		WORLD,
	};

	class AssetDatabase: public assetpackages::IAssetLoader {
	public:
		AssetDatabase();
		~AssetDatabase();
		auto AddAsset(std::string name, std::string path, AssetType type, std::filesystem::file_time_type time) -> std::string;
		auto UpdateAsset(std::string uuid, std::filesystem::file_time_type time) -> void;
		auto RemoveAsset(std::string uuid) -> void;
		auto Load(std::filesystem::path path) -> void; 
		auto GetModifiedTime(std::string uuid) -> std::time_t;
		auto GetUuidForPath(std::string path) -> std::string;
		auto LoadPackageList(std::string path) -> std::vector<assetpackages::AssetPackage*> override;
		auto CheckIfPackageHasAsset(const assetpackages::AssetPackage* package, std::string path) -> bool override;
		auto LoadPackage(std::string path) -> assetpackages::AssetPackage* override;
		auto LoadAsset(const assetpackages::AssetPackage* package, std::string path) -> std::vector<uint8_t> override;

	private:
		sqlite3* _database;

		auto CreateDatabase(std::filesystem::path path) -> void;
	};
}