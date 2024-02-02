#include "editor/core/loaders/FbxLoader.hxx"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

namespace kyanite::editor::loaders {
	FbxLoader::FbxLoader() {

	}

	FbxLoader::~FbxLoader() {
	}

	auto FbxLoader::LoadFromBuffer(std::vector<uint8_t> buffer) -> std::vector<engine::rendering::MeshData> {
		std::vector<engine::rendering::MeshData> meshes = {};

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFileFromMemory(buffer.data(), buffer.size(), 0, nullptr);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			throw std::runtime_error("Failed to load model");
		}

		std::vector<engine::rendering::Vertex> vertices;
		std::vector<uint32_t> indices;
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