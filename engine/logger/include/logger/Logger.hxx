#pragma once

#include "logger/ILogger.hxx"
#include <shared/Exported.hxx>

#include <memory>
#include <string_view>

namespace kyanite::engine::logging::logger {
	EXPORTED auto SetLogger(std::shared_ptr<ILogger> logger) -> void;
	EXPORTED auto Info(std::string_view message) -> void;
	EXPORTED auto Warn(std::string_view message) -> void;
	EXPORTED auto Error(std::string_view message) -> void;
	EXPORTED auto SetLogLevel(LogLevel level) -> void;
}