#pragma once

#include <cstdint>
#include <string>

namespace ResourceTracker {
	struct TrackedResource {
		std::string UUID;
		std::uint64_t RefID;
		std::uint16_t RefCount;
	};

	enum class TrackerExceptionCode {
		UNBOUND_RESOURCE = 1 
	};

	class TrackerException : public std::exception {
	public:
		TrackerExceptionCode Code;

		TrackerException(TrackerExceptionCode code, std::string message): exception(message.c_str()) {
			this->Code = code;
		}
	};

	auto Track(std::string uuid) -> void;
	auto Untrack(std::string uuid) -> void;
	auto RefIDFor(std::string uuid)->std::uint64_t;
}