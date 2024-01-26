#include "editor/FileWatchdog.hxx"

#include <chrono>
#include <filesystem>
#include <thread>

namespace kyanite::editor {
	FileWatchdog::FileWatchdog(
		std::filesystem::path path,
		std::chrono::milliseconds interval
	) : _path(path), _interval(interval) {
		// Check path for changes every interval milliseconds
		// Detect if a file has been added, removed, or modified
		// If any of the above, call the callback function with the path of the file and the type of change

		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			_files.push_back(entry);
		}
	}

	FileWatchdog::~FileWatchdog() {

	}

	auto FileWatchdog::Start() -> void {
		_watchThread = std::thread([this]() {
			while (_running) {
				std::this_thread::sleep_for(_interval);
				// Check for changes via filesystem
				// If changes, call callback
				// If no changes, continue
				std::vector<std::filesystem::directory_entry> currentEntries;

				for (const auto& entry : std::filesystem::directory_iterator(_path)) {
					currentEntries.push_back(entry);
				}

				// Query all files in currentEntries to see if they exist in _files or have been modified
				for (const auto& file : currentEntries) {
					if (std::find(_files.begin(), _files.end(), file) == _files.end()) {
						// File has been added
						_callback(FileEvent::ADDED, file);
					}
					else {
						// File does exist, check if it has been modified
						if (
							std::filesystem::last_write_time(file) !=
							std::filesystem::last_write_time(*std::find(_files.begin(), _files.end(), file))
							) {
							_callback(FileEvent::MODIFIED, file);
						}
					}
				}

				// Now we need to check if any files have been removed
				for (const auto& file : _files) {
					if (std::find(currentEntries.begin(), currentEntries.end(), file) == currentEntries.end()) {
						// File has been removed
						_callback(FileEvent::DELETED, file);
					}
				}

				_files = currentEntries;
			}
		});
	}

	auto FileWatchdog::Stop() -> void {

	}

	auto FileWatchdog::SetCallback(std::function<void(FileEvent, std::filesystem::directory_entry)> callback) -> void {
		_callback = callback;
	}
}