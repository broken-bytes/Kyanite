#pragma once

#include <string>
#include <vector>
#if _WIN32
#define NOMINMAX
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

	enum class ShaderAssetDescriptionPropType {
		TEXTURE = 0,
        BOOL    = 1,
        INT     = 2,
        FLOAT   = 3,
        VECTOR2 = 4,
        VECTOR3 = 5,
        VECTOR4 = 6
	};

	struct ShaderAssetDescriptionProp {
		std::string Name;
		ShaderAssetDescriptionPropType Type;
		uint8_t Slot;
	};

	struct ShaderAssetDescription {
		bool IsLit;
		std::vector<ShaderAssetDescriptionProp> Props;
	};

	struct ShaderAsset {
		std::string Name;
		ShaderAssetDescription Description;
		std::string Code;
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

	auto SetRootDir(std::string path) -> void;
	auto LoadModel(std::string path)-> ModelAsset;
	auto LoadShader(std::string path)->ShaderAsset;
	auto LoadTexture(std::string path)->TextureAsset;
}
