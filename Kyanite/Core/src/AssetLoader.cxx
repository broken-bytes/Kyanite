#include "AssetLoader.hxx"

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#if _WIN32
#include <windows.h>
#include <wrl.h>
#endif

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

#include "FreeImage.h"

using namespace std::filesystem;
namespace fs = std::filesystem;


namespace AssetLoader {
	auto LoadModel(std::string path)->ModelAsset {
		std::vector<ModelSubMesh> meshes = {};

		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		std::vector<uint8_t> bytes = {};

		bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
		std::stringstream outStream;
		for (auto& mesh : model.meshes) {
			for (auto& primitive : mesh.primitives) {
				std::vector<float> verts = {};
				std::vector<uint32_t> inds = {};
				const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
				const tinygltf::BufferView& indView = model.bufferViews[indAcc.bufferView];
				const tinygltf::Buffer& indBuff = model.buffers[indView.buffer];

				const tinygltf::Accessor& posAcc = model.accessors[primitive.attributes["POSITION"]];
				const tinygltf::Accessor& texAcc = model.accessors[primitive.attributes["TEXCOORD_0"]];
				const tinygltf::Accessor& norAcc = model.accessors[primitive.attributes["NORMAL"]];

				const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
				const tinygltf::BufferView& texView = model.bufferViews[texAcc.bufferView];
				const tinygltf::BufferView& norView = model.bufferViews[norAcc.bufferView];

				const tinygltf::Buffer& posBuff = model.buffers[posView.buffer];
				const tinygltf::Buffer& texBuff = model.buffers[texView.buffer];
				const tinygltf::Buffer& norBuff = model.buffers[norView.buffer];
				const float* positionsArr = reinterpret_cast<const float*>(&posBuff.data[posView.byteOffset + posAcc.byteOffset]);
				const float* texArr = reinterpret_cast<const float*>(&texBuff.data[texView.byteOffset + texAcc.byteOffset]);
				const float* norArr = reinterpret_cast<const float*>(&norBuff.data[norView.byteOffset + norAcc.byteOffset]);

				const uint32_t* indicesArr = reinterpret_cast<const uint32_t*>(&indBuff.data[indView.byteOffset + indAcc.byteOffset]);

				for (size_t i = 0; i < indAcc.count; ++i) {
					// Indices are uint32
					inds.push_back(indicesArr[i]);
				}

				for (size_t i = 0; i < posAcc.count; ++i) {

					float u = texArr[i * 2 + 0];
					float v = texArr[i * 2 + 1];

					if (u < 0) {
						u = 0;
					}

					if (u > 1) {
						u = 1;
					}
					if (v < 0) {
						v = 0;
					}

					if (v > 1) {
						v = 1;
					}

					verts.push_back(positionsArr[i * 3 + 0]);
					verts.push_back(positionsArr[i * 3 + 1]);
					verts.push_back(positionsArr[i * 3 + 2]);
					verts.push_back(1);
					verts.push_back(norArr[i * 3 + 0]);
					verts.push_back(norArr[i * 3 + 1]);
					verts.push_back(norArr[i * 3 + 2]);
					verts.push_back(u);
					verts.push_back(v);
				}

				ModelSubMesh subMesh = {mesh.name, verts, inds};
				meshes.push_back(subMesh);
			}
		}
		return { path, meshes };
	}

	auto LoadShader(const char* path)->ShaderAsset {
		std::ifstream file(path, std::ios::in);

		ShaderAsset asset = {};

		const auto fileSize = fs::file_size(path);

		// Create a buffer.
		std::string result(fileSize, '\0');

		asset.Code = new char[fileSize];

		// Read the whole file into the buffer.
		file.read(result.data(), fileSize);
		std::copy(result.begin(), result.end(), asset.Code);

		return asset;
	}

	auto CalcMipLevels(uint16_t width, uint16_t height) -> uint8_t {
		auto size = max(width, height);

		// FIXME: WRONG
		// DEBUGGING: Use 1 for now
		return 1;

		if (size >= 4096) {
			return 16;
		}
		else if (size >= 2048) {
			return 8;
		}
		else if (size >= 1024) {
			return 4;
		}
		else if (size >= 512) {
			return 2;
		}
	}

	auto LoadTexture(const char* path) -> TextureAsset {

		auto type = FreeImage_GetFileType(path);
		auto result = FreeImage_Load(type, path, type == FIF_PNG ? PNG_IGNOREGAMMA : 0);
		int width = FreeImage_GetWidth(result);
		int height = FreeImage_GetHeight(result);
		auto bbp = FreeImage_GetBPP(result);

		FreeImage_FlipVertical(result);
		RGBQUAD pix;
		FreeImage_GetPixelColor(result, 60, 0, &pix);

		int channels = bbp / 8;


		TextureAsset asset;

		for (int x = 1; x <= CalcMipLevels(width, height); x++) {
			TextureSubLevel level;
			auto data = new uint8_t[(width / x) * (height / x) * channels];
			auto scaled = FreeImage_Rescale(result, (width / x), (height / x), FILTER_BICUBIC);
			memcpy(data, (uint8_t*)FreeImage_GetBits(scaled), (width / x) * (height / x) * channels);
			FreeImage_Unload(scaled);

			level.Data = data;
			level.Width = width / x;
			level.Height = height / x;
			asset.Levels.push_back(level);
		}

		asset.Channels = channels;

		FreeImage_Unload(result);

		return asset;

		//*data = stbi_load(path, width, height, channels, STBI_rgb_alpha);
	}
}