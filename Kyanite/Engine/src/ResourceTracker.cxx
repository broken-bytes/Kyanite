#include "ResourceTracker.hxx"
#include <algorithm>
#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string.h>
#include <string>
#include <vector>


namespace ResourceTracker {
	std::vector<std::shared_ptr<TrackedResource>> Resources = {};
	auto Register(std::string uuid, uint64_t internalRefId, ResourceLifespan lifespan, std::function<void(uint64_t)> deleter) -> std::shared_ptr<TrackedResource> {
		auto resource = std::make_shared<TrackedResource>(uuid, internalRefId, 0, lifespan, deleter);
		return resource;
	}

	auto Track(std::string uuid) -> std::shared_ptr<TrackedResource> {
		auto resource = std::find_if(Resources.begin(), Resources.end(), [uuid](std::shared_ptr<TrackedResource> res) {
			return strcmp(uuid.c_str(), res->UUID.c_str());
		});

		if(resource != Resources.end()) {
			auto res = *resource;
			res->RefCount;
			return res;
		}

		throw std::runtime_error("Resource not bound");
	}

	auto Untrack(std::string uuid) -> void {
		auto resource = std::find_if(Resources.begin(), Resources.end(), [uuid](std::shared_ptr<TrackedResource> res) {
			return strcmp(uuid.c_str(), res->UUID.c_str());
		});

		if(resource != Resources.end()) {
			auto res = *resource;
			res->RefCount--;
			if(res->RefCount <= 0) {
				res->Deleter(res->RefID);
			}
			return;
		}

		throw std::runtime_error("Resource not bound");

	}
} // namespace ResourceTracker