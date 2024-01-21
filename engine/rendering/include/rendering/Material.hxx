#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include <glm/glm.hpp>

#include <cstdint>
#include <map>
#include <string>

namespace kyanite::engine::rendering {
    struct Material {
    public:
        uint64_t shaderId;
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
            Data(shaderId, textures, floats, ints, bools, vec2s, vec3s, vec4s);
        }
    };
}

