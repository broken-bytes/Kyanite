#include "assetpackages/AssetPackages.hxx"
#include "assetpackages/AssetPackageBuilder.hxx"
#include "assetpackages/AssetPackageLoader.hxx"
#include "assetpackages/AssetPackage.hxx"

#include <audio/AudioClip.hxx>
#include <rendering/Mesh.hxx>
#include <rendering/Texture.hxx>
#include <rendering/Shader.hxx>
#include <rendering/Material.hxx>

#include <algorithm>
#include <string>
#include <sstream>

namespace kyanite::engine::assetpackages {
	std::unique_ptr<IAssetLoader> assetLoader;

	auto Initialize(IAssetLoader* loader) -> void {
		if(loader != nullptr) {
			assetLoader = std::unique_ptr<IAssetLoader>(loader);
		} else {
			assetLoader = std::make_unique<AssetPackageLoader>();
		}
	}

	auto LoadPackage(const std::string path) -> AssetPackage* {
		return assetLoader->LoadPackage(path);
	}

	auto PackageHasAsset(const AssetPackage* package, std::string path) -> bool {
		return assetLoader->CheckIfPackageHasAsset(package, path);
	}

	template auto LoadAssetFromPackage(const AssetPackage* package, std::string path) -> audio::AudioClip;
	template auto LoadAssetFromPackage(const AssetPackage* package, std::string path) -> rendering::Material;
	template auto LoadAssetFromPackage(const AssetPackage* package, std::string path) -> rendering::Mesh;
	template auto LoadAssetFromPackage(const AssetPackage* package, std::string path) -> rendering::Shader;
	template auto LoadAssetFromPackage(const AssetPackage* package, std::string path) -> rendering::Texture;

	template <typename T>
	auto LoadAssetFromPackage(const AssetPackage* package, std::string path) -> T {
		T data;

		try {
			auto asset = AssetPackageLoader().LoadAsset(package, path);
			std::stringstream stream(std::string(asset.begin(), asset.end()));
			cereal::BinaryInputArchive archive(stream);
			archive(data);
		}
		catch (std::exception e) {
			throw std::runtime_error("Failed to load asset from package: " + std::string(e.what()));
		}

		return data;
	}

	auto CreateAssetPackage(std::string path) -> AssetPackage* {
		return AssetPackageBuilder().Create(path);
	}

	template auto AddToAssetPackage(AssetPackage* package, std::string path, audio::AudioClip data) -> void;
	template auto AddToAssetPackage(AssetPackage* package, std::string path, rendering::Material data) -> void;
	template auto AddToAssetPackage(AssetPackage* package, std::string path, rendering::Mesh data) -> void;
	template auto AddToAssetPackage(AssetPackage* package, std::string path, rendering::Shader data) -> void;
	template auto AddToAssetPackage(AssetPackage* package, std::string path, rendering::Texture data) -> void;

	template<typename T>
	auto AddToAssetPackage(AssetPackage* package, std::string path, T data) -> void {
		std::stringstream stream;
		cereal::BinaryOutputArchive archive(stream);
		archive(data);

		AssetPackageBuilder().AddAsset(package, path, stream.str());
	}
}