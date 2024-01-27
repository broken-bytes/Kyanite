#pragma once

#include <qwidget.h>

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

	class FileWatchdog: public QObject {
		Q_OBJECT;
	public:
		FileWatchdog(std::filesystem::path path, std::chrono::milliseconds interval);
		~FileWatchdog();

		auto Start() -> void;
		auto Stop() -> void;

	signals:
		auto FileAdded(std::filesystem::directory_entry file) -> void;
		auto FileDeleted(std::filesystem::directory_entry file) -> void;
		auto FileModified(std::filesystem::directory_entry file) -> void;

	private:
		std::thread _watchThread;
		std::filesystem::path _path;
		std::vector<std::filesystem::directory_entry> _files;
		bool _running;
		std::chrono::milliseconds _interval;
	};
}