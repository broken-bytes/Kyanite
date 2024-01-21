#pragma once

#include <cereal/access.hpp>

#include <cstdint>

namespace kyanite::engine::rendering {
	struct Mesh {
        uint32_t vertexBuffer;
		uint32_t indexBuffer;
		uint32_t vertexCount;
		uint32_t indexCount;

    private:
        friend class cereal::access;

        template <class Archive>
        void serialize(Archive& Data) {
            Data(vertexBuffer, indexBuffer, vertexCount, indexCount);
        }
	};
}