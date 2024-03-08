#pragma once

namespace kyanite::engine::rendering {
	enum class CommandListType {
		Graphics,
		Compute,
		Copy,
        Transfer
	};
}