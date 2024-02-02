#pragma once

#include <cstdint>
#include <map>
#include <tuple>
#include <string>
#include <vector>

namespace kyanite::editor::mappers {
	/**
		* @brief Interface for asset mappers.
		* @tparam T The type of the asset to convert to.
	*/
	template<typename M, typename D>
	class IMapper {
	public:
		virtual ~IMapper() = default;

		/**
			* @brief Converts the raw asset data into a format that can be used by the engine.
			* @param uuid The UUID of the asset.
			* @param data The raw asset data.
			* @return A pair containing the created metadata and the converted data.
		*/
		virtual auto Map(std::string uuid, std::vector<uint8_t> data) -> std::pair<M, D> = 0;
	};
}