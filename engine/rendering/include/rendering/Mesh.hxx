#pragma once

#include "Vertex.hxx"
#include <core/VendorSerializers/GlmSerializers.hxx>

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <glm/glm.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace kyanite::engine::rendering {
	struct MeshData {
		std::string name;
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive& Data) {
			Data(name, vertices, indices);
		}
	};

	struct Mesh {
		uint32_t vertexBuffer;
		uint32_t indexBuffer;
		uint32_t vertexCount;
		uint32_t indexCount;

	private:
		friend class cereal::access;

		template <class Archive>
		void serialize(Archive& Data) {
			Data(vertexBuffer, indexBuffer, vertexCount, indexCount);
		}
	};
}