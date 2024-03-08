#pragma once

#include <cstdint>

namespace kyanite::engine::rendering {
	struct Rect {
		uint32_t X;
		uint32_t Y;
		uint32_t Width;
		uint32_t Height;
	};
}