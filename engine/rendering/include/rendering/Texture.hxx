#pragma once

#include <glm/glm.hpp>

#include <cereal/cereal.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>


namespace kyanite::engine::rendering {
    struct Texture {
        uint64_t id;
        uint32_t width;
        uint32_t height;
        uint32_t channels;

        Texture() {
        	id = 0;
			width = 0;
			height = 0;
			channels = 0;
        }

        Texture(uint64_t id, uint32_t width, uint32_t height, uint32_t channels)
			: id(id), width(width), height(height), channels(channels) {
        }

    private:
        friend class cereal::access;

        template <class Archive>
        void serialize(Archive& Data) {
            Data(id, width, height, channels);
        }
    };
}

