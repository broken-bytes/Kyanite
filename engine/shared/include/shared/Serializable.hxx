#pragma once

#include <cstddef>
#include <concepts>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

namespace kyanite::engine::shared {
	template<typename T>
	concept Serializable = requires(T a)
	{
		{ std::vector<uint8_t> { } (a) } -> std::convertible_to<std::size_t>;
	};
}