#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <memory.h>

namespace ResourceTracker {
	enum class ResourceLifespan {
    	STATIC,
    	DYNAMIC,
	};

	struct TrackedResource {
		std::string UUID;
		std::uint64_t RefID;
		std::uint16_t RefCount;
		ResourceLifespan Lifespan;
		std::function<void(std::string)> Deleter;
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

	auto Register(std::string uuid) -> std::shared_ptr<TrackedResource>;
	auto Track(std::string uuid, ResourceLifespan lifespan) -> std::shared_ptr<TrackedResource>;
	auto Untrack(std::string uuid) -> void;
}