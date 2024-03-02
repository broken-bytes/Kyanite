#include "logger/ConsoleLogger.hxx"
#include "logger/ILogger.hxx"

#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/access.hpp>

#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace kyanite::engine::logging {
	ConsoleLogger::ConsoleLogger() {
        AllocConsole();
		SetStdHandle(STD_OUTPUT_HANDLE, GetStdHandle(STD_OUTPUT_HANDLE));
	}

	ConsoleLogger::~ConsoleLogger() {
		
	}

	auto ConsoleLogger::Info(std::string_view data) -> void {
		if (_level <= LogLevel::Info) {
			Log(LogLevel::Info, data);
		}
	}

	auto ConsoleLogger::Warn(std::string_view data) -> void {
		if (_level <= LogLevel::Warn) {
			Log(LogLevel::Info, data);
		}
	}

	auto ConsoleLogger::Error(std::string_view data) -> void {
		if (_level <= LogLevel::Error) {
			Log(LogLevel::Info, data);
		}
	}

	auto ConsoleLogger::Log(LogLevel level, std::string_view data) -> void {
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
		std::stringstream ss; {
			auto json = cereal::JSONOutputArchive(ss);
			json(
				cereal::make_nvp("level", logLevel),
				cereal::make_nvp("data", std::string(data)),
				cereal::make_nvp("time", timeStream.str())
			);
		}

		auto str = ss.str();

        // Output to console
		WriteConsoleA(
			GetStdHandle(STD_OUTPUT_HANDLE), 
			ss.str().c_str(), 
			ss.str().size(), 
			nullptr,
			nullptr
		);
	}

	auto ConsoleLogger::SetLogLevel(LogLevel level) -> void {
		_level = level;
	}
}