#pragma once

#include "../CommandAllocator.hxx"

namespace kyanite::engine::rendering {
	class GlCommandAllocator : public CommandAllocator {
	public:
		GlCommandAllocator() = default;
		~GlCommandAllocator() = default;

		virtual auto Reset() -> void {

		}
	};
}