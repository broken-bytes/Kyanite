#pragma once

#include <string>
#include <vector>
#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wrl.h>
#endif



namespace AssetLoader {
	struct ModelSubMesh {
		std::string Name;
		std::vector<float> Vertices;
		std::vector<uint32_t> Indicies;
	};

	struct ModelAsset {
		std::string Name;
		std::vector<ModelSubMesh> Meshes;
	};

	struct ShaderAsset {
		std::string RawData;
	};

	struct TextureSubLevel {
		uint16_t Width;
		uint16_t Height;
		uint8_t* Data;
	};

	struct TextureAsset {
		uint8_t Channels;
		std::vector<TextureSubLevel> Levels;
	};

	auto LoadModel(std::string path)-> ModelAsset;
	auto LoadShader(const char* path)->ShaderAsset;
	auto LoadTexture(const char* path)->TextureAsset;
}
