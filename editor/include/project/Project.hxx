#pragma once

#include <string>

namespace kyanite::editor {
	struct Project {
	public:
		std::string name;
		std::string description;
		std::string icon;
	};
}