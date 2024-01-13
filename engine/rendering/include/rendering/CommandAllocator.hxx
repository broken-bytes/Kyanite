#pragma once

namespace rendering {
	class CommandAllocator {
	public:
		virtual ~CommandAllocator() = default;
		virtual auto Reset() -> void = 0;
	};
}