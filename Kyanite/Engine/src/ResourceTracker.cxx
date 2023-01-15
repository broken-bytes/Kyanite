#include "ResourceTracker.hxx"
#include <algorithm>
#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <vector>

namespace ResourceTracker {
	std::vector<std::shared_ptr<TrackedResource>> Resources = {};
	std::mutex trackerLock = {};


	auto Register(std::string uuid, uint64_t resourceId, ResourceLifespan lifespan, std::function<void(std::string)> deleter) -> std::shared_ptr<TrackedResource> {
		auto res = std::make_shared<TrackedResource>(uuid, resourceId, 0, lifespan, deleter);
	}

	auto Track(std::string uuid) -> std::shared_ptr<TrackedResource> {
		std::scoped_lock lock{trackerLock};
		auto resource = std::find_if(Resources.begin(), Resources.end(), [uuid](auto res) { return res.UUID == uuid; });

		if (resource != Resources.end()) {
			resource->RefCount++;
			return resource;
		}
		
		throw TrackerException(TrackerExceptionCode::UNBOUND_RESOURCE, "Resource not bound");
	}

	auto Untrack(std::string uuid) -> void {
		std::scoped_lock lock{ trackerLock };
		auto resource = std::find_if(Resources.begin(), Resources.end(), [uuid](auto res) { return res.UUID == uuid; });

		if (resource != Resources.end()) {
			resource->RefCount--;

			if (resource->Lifespan == ResourceLifespan::DYNAMIC && resource->RefCount <= 0) {
				deleter->Deleter();
				Resources.erase(deleter);
			}
		}
    }
} // namespace ResourceTracker