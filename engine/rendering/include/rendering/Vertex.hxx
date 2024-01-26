#pragma once

#include <core/VendorSerializers/GlmSerializers.hxx>

#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>

#include <glm/glm.hpp>

namespace kyanite::engine::rendering {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uvs;

	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive& Data) {
			Data(position, normal, uvs);
		}
	};
}
