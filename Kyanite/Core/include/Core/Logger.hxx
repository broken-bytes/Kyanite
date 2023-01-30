#pragma once

#include <string>
#include <string_view>
#include <source_location>

namespace Logger {
	auto Init() -> void;
	auto Info(std::string content, const std::source_location& location = std::source_location::current()) -> void;
	auto Warn(std::string content, const std::source_location& location = std::source_location::current()) -> void;
	auto Error(std::string content, const std::source_location& location = std::source_location::current()) -> void;
}