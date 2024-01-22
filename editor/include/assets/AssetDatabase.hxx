#pragma once

#include <string>

namespace kyanite::editor {
	class AssetDatabase {
	public:
		AssetDatabase(std::string path);
		~AssetDatabase();

		auto LoadAsset(std::string path) -> void;
	};
}