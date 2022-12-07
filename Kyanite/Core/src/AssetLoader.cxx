#include "AssetLoader.hxx"

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#if _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <wrl.h>
#endif

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE

#include <tiny_gltf.h>

#include "FreeImage.h"
#include <yaml-cpp/yaml.h>

using namespace std::filesystem;
namespace fs = std::filesystem;

namespace AssetLoader {

// Helper functions & types
std::map<std::string, ShaderAssetDescriptionPropType> TypeMappings = {
	{"Texture", ShaderAssetDescriptionPropType::TEXTURE},
	{"Int", ShaderAssetDescriptionPropType::INT},
	{"Float", ShaderAssetDescriptionPropType::FLOAT},
	{"Vector2", ShaderAssetDescriptionPropType::VECTOR2},
	{"Vector3", ShaderAssetDescriptionPropType::VECTOR3},
	{"Vector4", ShaderAssetDescriptionPropType::VECTOR4},
	{"Bool", ShaderAssetDescriptionPropType::BOOL},
};

auto ShaderPropTypeNameToType(const std::string& type) -> ShaderAssetDescriptionPropType {
	return TypeMappings.at(type);
}

auto ShaderPropTypeToName(ShaderAssetDescriptionPropType type) -> std::string {
	auto item = std::find_if(TypeMappings.begin(), TypeMappings.end(), [type](auto& item) {
		return item.second == type;
	});
	
	return item->first;
}


std::string RootDir = "";

	auto SetRootDir(std::string path) -> void {
		RootDir = path;
	}

	auto LoadModel(std::string path)->ModelAsset {
		std::vector<ModelSubMesh> meshes = {};

		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;

		std::vector<uint8_t> bytes = {};

		auto fullPath = RootDir + path;

		bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, fullPath);
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
				const tinygltf::Accessor& colAcc = model.accessors[primitive.attributes["COLOR"]];

				const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
				const tinygltf::BufferView& texView = model.bufferViews[texAcc.bufferView];
				const tinygltf::BufferView& norView = model.bufferViews[norAcc.bufferView];
				const tinygltf::BufferView& colView = model.bufferViews[colAcc.bufferView];

				const tinygltf::Buffer& posBuff = model.buffers[posView.buffer];
				const tinygltf::Buffer& texBuff = model.buffers[texView.buffer];
				const tinygltf::Buffer& norBuff = model.buffers[norView.buffer];
				const tinygltf::Buffer& colBuff = model.buffers[colView.buffer];
				const float* positionsArr = reinterpret_cast<const float*>(&posBuff.data[posView.byteOffset + posAcc.byteOffset]);
				const float* texArr = reinterpret_cast<const float*>(&texBuff.data[texView.byteOffset + texAcc.byteOffset]);
				const float* norArr = reinterpret_cast<const float*>(&norBuff.data[norView.byteOffset + norAcc.byteOffset]);
				const float* colArr = reinterpret_cast<const float*>(&colBuff.data[colView.byteOffset + colAcc.byteOffset]);

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
					auto r = colArr[i * 3 + 0];
					auto g = colArr[i * 3 + 1];
					auto b = colArr[i * 3 + 2];

					verts.push_back(r);
					verts.push_back(g);
					verts.push_back(b);
					verts.push_back(1);
				}

				ModelSubMesh subMesh = {mesh.name, verts, inds};
				meshes.push_back(subMesh);
			}
		}
		return { path, meshes };
	}

        auto LoadShader(std::string path) -> ShaderAsset {
                auto fullPath = RootDir + path;
                ShaderAsset asset = {};

                // Gather shader description
                YAML::Node shaderDescription = YAML::LoadFile(fullPath);
                const std::string name =
                    shaderDescription["name"].as<std::string>();
                const std::string hlslPath =
                    RootDir + shaderDescription["path"].as<std::string>();
				std::string lighting = shaderDescription["lighting"].as<std::string>();
				asset.Description.IsLit = lighting == "default" ? true : false;
				
				std::string format = shaderDescription["format"].as<std::string>();

				if(format == "rgba_float") {
					asset.Description.Format = ShaderAssetOutputFormat::RGBA_FLOAT;
				}

				if(format == "rgba_uint") {
					asset.Description.Format = ShaderAssetOutputFormat::RGBA_UINT;
				}


                const auto constants = shaderDescription["constants"];

                for (YAML::const_iterator it = constants.begin();
                     it != constants.end(); ++it) {
                        const YAML::Node &inputElem = *it;
                        std::string name = inputElem["name"].as<std::string>();
                        std::string type = inputElem["type"].as<std::string>();
                        uint8_t slot = inputElem["slot"].as<int>();
                        ShaderAssetDescriptionProp prop = {};
                        prop.Type = ShaderPropTypeNameToType(type);
                        prop.Slot = slot;
                        prop.Name = name;
                        asset.Description.Constants.push_back(prop);
                }

				const auto buffer = shaderDescription["buffer"];

                for (YAML::const_iterator it = buffer.begin();
                     it != buffer.end(); ++it) {
                        const YAML::Node &inputElem = *it;
                        std::string name = inputElem["name"].as<std::string>();
                        std::string type = inputElem["type"].as<std::string>();
                        uint8_t slot = inputElem["slot"].as<int>();
                        ShaderAssetDescriptionProp prop = {};
                        prop.Type = ShaderPropTypeNameToType(type);
                        prop.Slot = slot;
                        prop.Name = name;
                        asset.Description.ConstantBufferLayout.push_back(prop);
                }

				asset.Name = name;
                const auto shaderCodeFileSize = fs::file_size(hlslPath);

                // Create a buffer.
                std::string shaderCode(shaderCodeFileSize, '\0');
                std::ifstream file(hlslPath, std::ios::in);

                // Read the whole file into the buffer.
                file.read(shaderCode.data(), shaderCodeFileSize);
                asset.Code = shaderCode;

                return asset;
        }

        auto CalcMipLevels(uint16_t width, uint16_t height) -> uint8_t {
			
		auto size = std::max(width, height);

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

	auto LoadTexture(std::string path) -> TextureAsset {
		auto fullPath = RootDir + path;

		auto type = FreeImage_GetFileType(fullPath.c_str());
		auto result = FreeImage_Load(type, fullPath.c_str(), type == FIF_PNG ? PNG_IGNOREGAMMA : 0);
		int width = FreeImage_GetWidth(result);
		int height = FreeImage_GetHeight(result);
		auto bbp = FreeImage_GetBPP(result);

		FreeImage_FlipVertical(result);
		RGBQUAD pix;
		FreeImage_GetPixelColor(result, 60, 0, &pix);

		int channels = bbp / 8;

		if(channels != 4) {
			result = FreeImage_ConvertTo32Bits(result);
		}

		channels = 4;


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