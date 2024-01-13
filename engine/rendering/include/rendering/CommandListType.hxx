#pragma once

namespace rendering {
	enum class CommandListType {
		Graphics,
		Compute,
		Copy,
        Transfer
	};
}