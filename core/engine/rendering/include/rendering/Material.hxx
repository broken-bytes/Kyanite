#pragma once

#include "Shader.hxx"

#include <cereal/cereal.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include <glm/glm.hpp>

#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace kyanite::engine::rendering {
	struct MaterialData {
	public:
		uint32_t vertexShaderId;
		uint32_t pixelShaderId;

		std::map<std::string, uint64_t> textures;
		std::map<std::string, float> floats;
		std::map<std::string, uint32_t> ints;
		std::map<std::string, bool> bools;
		std::map<std::string, std::array<float, 2>> vec2s;
		std::map<std::string, std::array<float, 3>> vec3s;
		std::map<std::string, std::array<float, 4>> vec4s;

	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive& Data) {
			Data(vertexShaderId, pixelShaderId, textures, floats, ints, bools, vec2s, vec3s, vec4s);
		}
	};

	class Material {
	public:
		std::map<ShaderType, std::shared_ptr<Shader>> shaders;
		std::map<std::string, uint64_t> textures;
		std::map<std::string, float> floats;
		std::map<std::string, uint32_t> ints;
		std::map<std::string, bool> bools;
		std::map<std::string, std::array<float, 2>> vec2s;
		std::map<std::string, std::array<float, 3>> vec3s;
		std::map<std::string, std::array<float, 4>> vec4s;

		Material(
			std::map<ShaderType, std::shared_ptr<Shader>> shaders,
			std::map<std::string, uint64_t> textures,
			std::map<std::string, float> floats,
			std::map<std::string, uint32_t> ints,
			std::map<std::string, bool> bools,
			std::map<std::string, std::array<float, 2>> vec2s,
			std::map<std::string, std::array<float, 3>> vec3s,
			std::map<std::string, std::array<float, 4>> vec4s
		) : shaders(shaders), 
			textures(textures), 
			floats(floats), 
			ints(ints), 
			bools(bools), 
			vec2s(vec2s), 
			vec3s(vec3s), 
			vec4s(vec4s) {}
	};
}

