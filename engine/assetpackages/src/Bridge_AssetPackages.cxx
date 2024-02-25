#include "assetpackages/Bridge_AssetPackages.h"
#include "assetpackages/AssetPackages.hxx"
#include "assetpackages/AssetPackageLoader.hxx"
#include "assetpackages/IAssetLoader.hxx"
#include <audio/AudioClip.hxx>
#include <rendering/Material.hxx>
#include <rendering/Model.hxx>
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

void AssetPackages_LoadAssetPackages(const char* path, NativePointer* buffer, size_t* numPackages) {
	auto packages = assetpackages::LoadPackageList(path);
	*numPackages = packages.size();

	auto packageBuffer = new NativePointer[*numPackages];
	for (size_t i = 0; i < *numPackages; i++) {
		packageBuffer[i] = reinterpret_cast<NativePointer>(&packages[i]);
	}

	*buffer = packageBuffer;
}

NativePointer AssetPackages_LoadTexture(NativePointer assetPackage, const char* uuid) {
	auto package = reinterpret_cast<assetpackages::AssetPackage*>(assetPackage);
	auto texture = assetpackages::LoadAssetFromPackage<rendering::Texture>(package, uuid);

	rendering::Texture* texturePtr = new rendering::Texture;
	*texturePtr = texture;

	return reinterpret_cast<NativePointer>(texturePtr);
}

void AssetPackages_FreeTexture(NativePointer pointer) {
	delete reinterpret_cast<rendering::Texture*>(pointer);
}

void AssetPackages_LoadModel(NativePointer assetPackage, const char* uuid, uint8_t** ids, size_t* numMeshes) {
	auto package = reinterpret_cast<assetpackages::AssetPackage*>(assetPackage);
	auto model = assetpackages::LoadAssetFromPackage<rendering::ModelData>(package, uuid);

	auto idsPtr = new uint8_t[model.meshes.size()];
	// Copy mesh ids to array and convert to uint8_t
	std::copy(
		reinterpret_cast<uint8_t*>(model.meshes.data()),
		reinterpret_cast<uint8_t*>(model.meshes.data()) + model.meshes.size(),
		idsPtr
	);

	*ids = idsPtr;
	*numMeshes = model.meshes.size();
}

void AssetPackages_LoadMesh(
	NativePointer assetPackage,
	const char* uuid,
	uint8_t id,
	float** vertices,
	size_t* numVertices,
	uint32_t** indices,
	size_t* numIndices
) {
	auto package = reinterpret_cast<assetpackages::AssetPackage*>(assetPackage);
	auto model = assetpackages::LoadAssetFromPackage<rendering::ModelData>(package, uuid);

	auto mesh = model.meshes[id];

	auto numFloats = mesh.vertices.size() * sizeof(rendering::Vertex) / sizeof(float);
	auto verticesPtr = new float[mesh.vertices.size() * sizeof(rendering::Vertex) / sizeof(float)];
	auto indicesPtr = new uint32_t[mesh.indices.size()];

	//std::copy(mesh.vertices.begin(), mesh.vertices.end(), verticesPtr);
	// Copy vertices to float array. Need to convert from Vertex to float
	std::copy(
		reinterpret_cast<float*>(mesh.vertices.data()),
		reinterpret_cast<float*>(mesh.vertices.data()) + numFloats,
		verticesPtr
	);

	std::copy(mesh.indices.begin(), mesh.indices.end(), indicesPtr);

	*vertices = verticesPtr;
	*indices = indicesPtr;
	*numVertices = numFloats;
	*numIndices = mesh.indices.size();
}

void AssetPackages_FreeMesh(NativePointer pointer) {
	delete reinterpret_cast<rendering::MeshData*>(pointer);
}

NativePointer AssetPackages_LoadAudioClip(NativePointer assetPackage, const char* uuid) {
	auto package = reinterpret_cast<assetpackages::AssetPackage*>(assetPackage);
	auto clip = assetpackages::LoadAssetFromPackage<audio::AudioClip>(package, uuid);

	audio::AudioClip* clipPtr = new audio::AudioClip;
	*clipPtr = clip;

	return reinterpret_cast<NativePointer>(clipPtr);
}

void AssetPackages_FreeAudioClip(NativePointer pointer) {
	delete reinterpret_cast<audio::AudioClip*>(pointer);
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