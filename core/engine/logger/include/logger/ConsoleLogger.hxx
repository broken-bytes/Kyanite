#pragma once

#include "ILogger.hxx"

#include <fstream>
#include <string_view>

namespace kyanite::engine::logging {
	class ConsoleLogger : public ILogger {
	public:
		ConsoleLogger();
		~ConsoleLogger() override;
		auto Info(std::string_view data) -> void override;
		auto Warn(std::string_view data) -> void override;
		auto Error(std::string_view data) -> void override;
		auto Log(LogLevel level, std::string_view data) -> void override;
		auto SetLogLevel(LogLevel level) -> void override;

	private:
		LogLevel _level;
	};
}