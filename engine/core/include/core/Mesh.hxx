#pragma once

#include <cstdint>

namespace core {
	struct Mesh {
        uint32_t vertexBuffer;
		uint32_t indexBuffer;
		uint32_t vertexCount;
		uint32_t indexCount;
	};
}