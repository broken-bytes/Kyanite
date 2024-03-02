#include "logger/Logger.hxx"
#include "logger/ILogger.hxx"

namespace kyanite::engine::logging::logger {
	std::shared_ptr<ILogger> logger;
	
	auto SetLogger(std::shared_ptr<ILogger> logger) -> void {
		kyanite::engine::logging::logger::logger = logger;
	}

	auto Info(std::string_view message) -> void {
		logger->Info(message);
	}

	auto Warn(std::string_view message) -> void {
		logger->Warn(message);
	}

	auto Error(std::string_view message) -> void {
		logger->Error(message);
	}

	auto SetLogLevel(LogLevel level) -> void {
		logger->SetLogLevel(level);
	}
}