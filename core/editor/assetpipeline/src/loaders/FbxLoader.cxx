#include "assetpipeline/loaders/FbxLoader.hxx"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

namespace kyanite::editor::assetpipeline::loaders {
	FbxLoader::FbxLoader() {

	}

	FbxLoader::~FbxLoader() {
	}

	auto FbxLoader::LoadFromFile(
		const char* path
	) -> std::vector<kyanite::engine::rendering::MeshData> {
		std::vector<engine::rendering::MeshData> meshes = {};

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, 0);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			auto err = importer.GetErrorString();
			throw std::runtime_error(err);
		}

		std::vector<engine::rendering::Vertex> vertices;
		std::vector<uint32_t> indices = {};

		// Process animations
		for (int x = 0; x < scene->mNumAnimations; x++) {
			aiAnimation* animation = scene->mAnimations[x];
			// Process channels
			for (int i = 0; i < animation->mNumChannels; i++) {
				aiNodeAnim* channel = animation->mChannels[i];
				// Process keyframes
				for (int j = 0; j < channel->mNumPositionKeys; j++) {
					aiVectorKey key = channel->mPositionKeys[j];
					// Process keyframe
				}
			}
		}

		for (int x = 0; x < scene->mNumMeshes; x++) {
			// Process mesh
			aiMesh* mesh = scene->mMeshes[x];

			// Fill vertices
			for (int i = 0; i < mesh->mNumVertices; i++) {
				engine::rendering::Vertex vertex;
				vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				if (mesh->mTextureCoords[0]) {
					vertex.uvs = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				}
				else {
					vertex.uvs = glm::vec2(0.0f, 0.0f);
				}
				vertices.push_back(vertex);
			}

			// Fill indices
			for (int i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for (int j = 0; j < face.mNumIndices; j++) {
					indices.push_back(face.mIndices[j]);
				}
			}

			// Create mesh
			engine::rendering::MeshData meshData;
			meshData.vertices = vertices;
			meshData.indices = indices;
			meshes.push_back(meshData);

			// Clear vectors
			vertices.clear();
			indices.clear();
		}

		return meshes;
	}
}