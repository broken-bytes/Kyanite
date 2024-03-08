#pragma once

#include "ILogger.hxx"

#include <fstream>
#include <string_view>

namespace kyanite::engine::logging {
	class FileLogger : public ILogger {
	public:
		FileLogger(std::string_view path);
		~FileLogger() override;
		auto Info(std::string_view data) -> void override;
		auto Warn(std::string_view data) -> void override;
		auto Error(std::string_view data) -> void override;
		auto Log(LogLevel level, std::string_view data) -> void override;
		auto SetLogLevel(LogLevel level) -> void override;

	private:
		std::string _path;
		std::ofstream _file;
		LogLevel _level;
	};
}