#pragma once

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>

#include <string>

namespace kyanite::editor {
	struct Project {
	public:
		std::string name;
		std::string description;
		std::string icon;

	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive& archive) {
			archive(cereal::make_nvp("name", name), cereal::make_nvp("description", description), cereal::make_nvp("icon", icon));
		}
	};
}