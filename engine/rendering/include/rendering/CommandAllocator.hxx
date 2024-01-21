#pragma once

namespace kyanite::engine::rendering {
	class CommandAllocator {
	public:
		virtual ~CommandAllocator() = default;
		virtual auto Reset() -> void = 0;
	};
}