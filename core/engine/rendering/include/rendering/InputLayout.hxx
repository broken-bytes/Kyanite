#pragma once

#include "Format.hxx"

#include <string>

namespace kyanite::engine::rendering {
	struct InputLayout {
		std::string name;
		uint32_t index;
		Format format;
		uint32_t slot;
		uint32_t offset;
		uint32_t inputSlotClass;
		uint32_t instanceDataStepRate;
	};
} // namespace kyanite::engine::rendering