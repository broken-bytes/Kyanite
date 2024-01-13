#pragma once

#include <cstdint>

namespace rendering {
	struct Viewport {
		uint32_t X;
		uint32_t Y;
		uint32_t Width;
		uint32_t Height;
		float MinDepth;
		float MaxDepth;
	};
}