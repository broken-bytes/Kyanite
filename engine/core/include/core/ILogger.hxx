#pragma once

#include <string_view>

enum class LogLevel {
	Info,
	Warn,
	Error
};

namespace kyanite::engine::core {
	class ILogger {
	public:
		virtual ~ILogger() = default;

		/**
		* @brief Log a message with the info level
		*
		* @param data The message to log
		*/
		virtual auto Info(std::string_view data) -> void = 0;

		/**
		* @brief Log a message with the warning level
		*
		* @param data The message to log
		*/
		virtual auto Warn(std::string_view data) -> void = 0;

		/**
		* @brief Log a message with the error level
		*
		* @param data The message to log
		*/
		virtual auto Error(std::string_view data) -> void = 0;

		/**
		* @brief Log a message with the info level
		*
		* @param level The level of the message
		* @param data The message to log
		*/
		virtual auto Log(LogLevel level, std::string_view data) -> void = 0;

		/**
		*	@brief Set the minimal log level to log
		*	@param level The new log level
		*
		*	@note The default log level is Info
		*/
		virtual auto SetLogLevel(LogLevel level) -> void = 0;
	};
}