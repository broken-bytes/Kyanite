#include "Logger.hxx"
#include "plog/Log.h"
#include "plog/Initializers/ConsoleInitializer.h"
#include "plog/Initializers/RollingFileInitializer.h"
#include <string_view>
#include <source_location>

#ifdef _WIN32
#include <Windows.h>
#endif
static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender; // Create the 2nd appender.

namespace Logger {

	auto Init() -> void {
		plog::init(plog::verbose, &consoleAppender);
	}

	auto Info(std::string content, const std::source_location& location) -> void {
		PLOG_INFO << content.c_str();
	}

	auto Warn(std::string content, const std::source_location& location) -> void {
		PLOG_ERROR << content.c_str();
	}

	auto Error(std::string content, const std::source_location& location) -> void {
		PLOG_FATAL << content.c_str();
	}
}