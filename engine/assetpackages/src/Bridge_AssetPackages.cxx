#include "assetpackages/Bridge_AssetPackages.h"
#include "assetpackages/AssetPackages.hxx"
#include "assetpackages/AssetPackageLoader.hxx"
#include "assetpackages/IAssetLoader.hxx"
#include <audio/AudioClip.hxx>
#include <rendering/Material.hxx>
#include <rendering/Mesh.hxx>
#include <rendering/Texture.hxx>

#include <sstream>

namespace assetpackages = kyanite::engine::assetpackages;
namespace audio = kyanite::engine::audio;
namespace rendering = kyanite::engine::rendering;

void AssetPackages_Initialize(NativePointer loader) {
	auto assetLoaderPtr = reinterpret_cast<assetpackages::IAssetLoader*>(loader);
	auto smartLoader = std::shared_ptr<assetpackages::IAssetLoader>(assetLoaderPtr);
	assetpackages::Initialize(smartLoader);
}

NativePointer AssetPackages_LoadAssetPackages(const char* path, size_t* numPackages) {
	auto packages = assetpackages::LoadPackageList(path);
	*numPackages = packages.size();

	assetpackages::AssetPackage** packagesPtr = new assetpackages::AssetPackage*[*numPackages];

	for(size_t x = 0; x < *numPackages; x++) {
		packagesPtr[x] = packages[x];
	}

	return reinterpret_cast<NativePointer>(packagesPtr);
}

NativePointer AssetPackages_LoadTexture(NativePointer assetPackage, const char* uuid) {
	auto package = reinterpret_cast<assetpackages::AssetPackage*>(assetPackage);
	auto texture = assetpackages::LoadAssetFromPackage<rendering::Texture>(package, uuid);

	rendering::Texture* texturePtr = new rendering::Texture;
	*texturePtr = texture;

	return reinterpret_cast<NativePointer>(texturePtr);
}

NativePointer AssetPackages_LoadMesh(NativePointer assetPackage, const char* uuid) {
	auto package = reinterpret_cast<assetpackages::AssetPackage*>(assetPackage);
	auto mesh = assetpackages::LoadAssetFromPackage<rendering::Mesh>(package, uuid);
	
	rendering::Mesh* meshPtr = new rendering::Mesh;
	*meshPtr = mesh;

	return reinterpret_cast<NativePointer>(meshPtr);
}

NativePointer AssetPackages_LoadAudioClip(NativePointer assetPackage, const char* uuid) {
	auto package = reinterpret_cast<assetpackages::AssetPackage*>(assetPackage);
	auto clip = assetpackages::LoadAssetFromPackage<audio::AudioClip>(package, uuid);

	audio::AudioClip* clipPtr = new audio::AudioClip;
	*clipPtr = clip;

	return reinterpret_cast<NativePointer>(clipPtr);
}

NativePointer AssetPackages_LoadMaterial(NativePointer assetPackage, const char* uuid) {
	return nullptr;
}

NativePointer AssetPackages_LoadShader(NativePointer assetPackage, const char* uuid) {
	return nullptr;
}

void AssetPackages_DisposeAsset(NativePointer asset) {
	delete asset;
}