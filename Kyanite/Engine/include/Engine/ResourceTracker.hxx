#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <memory>

namespace ResourceTracker {
	enum class ResourceLifespan {
    	STATIC,
    	DYNAMIC,
	};

	struct TrackedResource {
		std::string UUID;
		uint64_t RefID;
		uint16_t RefCount;
		ResourceLifespan Lifespan;
		std::function<void(uint64_t)> Deleter;
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

	auto Register(std::string uuid, uint64_t internalRefId, ResourceLifespan lifespan, std::function<void(uint64_t)> deleter) -> std::shared_ptr<TrackedResource>;
	auto Track(std::string uuid) -> std::shared_ptr<TrackedResource>;
	auto Untrack(std::string uuid) -> void;
}