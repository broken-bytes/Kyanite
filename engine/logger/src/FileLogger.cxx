#include "logger/FileLogger.hxx"
#include "logger/ILogger.hxx"

#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/access.hpp>

#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace kyanite::engine::logging {
	FileLogger::FileLogger(std::string_view path) {
		_path = path;
		_file = std::ofstream(_path.data(), std::ios::out | std::ios::app);
	}

	FileLogger::~FileLogger() {
		_file.close();
	}

	auto FileLogger::Info(std::string_view data) -> void {
		if (_level <= LogLevel::Info) {
			Log(LogLevel::Info, data);
		}
	}

	auto FileLogger::Warn(std::string_view data) -> void {
		if (_level <= LogLevel::Warn) {
			Log(LogLevel::Info, data);
		}
	}

	auto FileLogger::Error(std::string_view data) -> void {
		if (_level <= LogLevel::Error) {
			Log(LogLevel::Info, data);
		}
	}

	auto FileLogger::Log(LogLevel level, std::string_view data) -> void {
		std::stringstream ss;
		auto json = cereal::JSONOutputArchive(ss);

		std::stringstream timeStream;

		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);
		auto time = std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
		timeStream << time;
		
		std::string logLevel = "info";

		switch(level) {
			case LogLevel::Info:
				logLevel = "info";
				break;
			case LogLevel::Warn:
				logLevel = "warn";
				break;
			case LogLevel::Error:
				logLevel = "error";
				break;
		}

		json(
			cereal::make_nvp("level", logLevel),
			cereal::make_nvp("data", std::string(data)),
			timeStream.str()
		);

		_file << ss.str() << std::endl;
	}

	auto FileLogger::SetLogLevel(LogLevel level) -> void {
		_level = level;
	}
}