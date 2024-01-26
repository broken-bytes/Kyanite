#pragma once

#include <chrono>
#include <filesystem>
#include <functional>
#include <map>
#include <thread>

namespace kyanite::editor {
	enum class FileEvent {
		ADDED,
		DELETED,
		MODIFIED
	};

	class FileWatchdog {
	public:
		FileWatchdog(std::filesystem::path path, std::chrono::milliseconds interval);
		~FileWatchdog();

		auto Start() -> void;
		auto Stop() -> void;

		auto SetCallback(std::function<void(FileEvent, std::filesystem::directory_entry)> callback) -> void;

	private:
		std::thread _watchThread;
		std::filesystem::path _path;
		std::vector<std::filesystem::directory_entry> _files;
		std::function<void(FileEvent, std::filesystem::directory_entry)> _callback;
		bool _running;
		std::chrono::milliseconds _interval;
	};
}