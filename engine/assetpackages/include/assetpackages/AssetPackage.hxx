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
	struct Asset {
		std::string path;
		std::string type;
		size_t size;
		size_t offset;

		Asset() = default;

		Asset(std::string_view path, std::string_view type, size_t size, size_t offset) :
			path(path),
			type(type),
			size(size),
			offset(offset) {}

	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive& ar) {
			ar(path, type, size, offset);
		}
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