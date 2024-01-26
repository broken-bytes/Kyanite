#pragma once

#include <shared/Exported.hxx>

#include <cereal/access.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <nlohmann/json.hpp>

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace kyanite::engine::assetpackages {
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

	struct Asset {
		std::string uuid;
		std::string name;
		std::string path;
		AssetType type;
		int64_t time;

		Asset() = default;

		Asset(std::string_view uuid, std::string name, std::string_view path, AssetType type, int64_t time) :
			uuid(uuid),
			name(name),
			path(path),
			type(type),
			time(time) {}
	};

	// Asset packages are binary storage files for assets.
	// They are used to store assets in a single file, and are used for distribution.
	// The file format is as follows:
	// The first part of the file is a header, which contains the following:
	// JSON data containing:
	// - The asset package's metadata, such as the name, version, and author.
	// - A list of assets, which contains the following:
	//   - The asset's name.
	//   - The asset's type.
	//   - The asset's size.
	//   - The asset's offset in the file.
	// The second part of the file is the actual asset data.
	class AssetPackage {
	public:
		AssetPackage() {
		}
		~AssetPackage();

		template<typename T>
		auto LoadAsset(const std::string_view path) -> void {
			//static_assert(false, "AssetPackage::LoadAsset<T> is not implemented for this type");
		}

	private:
		friend class AssetPackageBuilder;
		friend class AssetPackageLoader;

		std::string _path;

		AssetPackage(std::string path);
	};
}