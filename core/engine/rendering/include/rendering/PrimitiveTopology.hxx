#pragma once

namespace kyanite::engine::rendering {
	enum class PrimitiveTopology {
		POINT_LIST,
		LINE_LIST,
		LINE_STRIP,
		TRIANGLE_LIST,
		TRIANGLE_STRIP,
		LINE_LIST_ADJACENCY,
		LINE_STRIP_ADJACENCY,
		TRIANGLE_LIST_ADJACENCY,
		TRIANGLE_STRIP_ADJACENCY,
		PATCH_LIST
	};
}