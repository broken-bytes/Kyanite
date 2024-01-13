#pragma once

#include <glm/glm.hpp>

#include <cstdint>
#include <map>
#include <string>

namespace core {
    struct Material {
        uint64_t shaderId;
        std::map<std::string_view, uint64_t> textures;
        std::map<std::string_view, float> floats;
        std::map<std::string_view, uint32_t> ints;
        std::map<std::string_view, bool> bools;
        std::map<std::string_view, glm::vec2> vec2s;
        std::map<std::string_view, glm::vec3> vec3s;
        std::map<std::string_view, glm::vec4> vec4s;
    };
}

