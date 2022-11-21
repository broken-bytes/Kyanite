#include "ResourceTracker.hxx"
#include <algorithm>
#include <mutex>
#include <stdexcept>
#include <vector>

namespace ResourceTracker {
	std::vector<TrackedResource> Resources = {};
	std::mutex trackerLock = {};

	auto Track(std::string uuid) -> void {
		std::scoped_lock lock{trackerLock};
		auto resource = std::find_if(Resources.begin(), Resources.end(), [uuid](auto res) { return res.UUID == uuid; });

		if (resource != Resources.end()) {
			resource->RefCount++;
		}
		else {
			throw TrackerException(TrackerExceptionCode::UNBOUND_RESOURCE, "Resource not bound");
		}
	}

	auto Untrack(std::string uuid) -> void {
		std::scoped_lock lock{ trackerLock };
		auto resource = std::find_if(Resources.begin(), Resources.end(), [uuid](auto res) { return res.UUID == uuid; });

		if (resource != Resources.end()) {
			resource->RefCount--;
		}
	}

	auto RefIDFor(std::string uuid) -> std::uint64_t {
		auto resource = std::find_if(Resources.begin(), Resources.end(), [uuid](auto res) { return res.UUID == uuid; });

		if (resource != Resources.end()) {
			return resource->RefID;
		}

		throw TrackerException(TrackerExceptionCode::UNBOUND_RESOURCE, "Resource not bound");
	}
}