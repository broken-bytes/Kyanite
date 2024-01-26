#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/map.hpp>

namespace glm {

	template<class Archive>
	void serialize(Archive& archive, glm::vec2& m)
	{
		archive(m.x, m.y);
	}

	template<class Archive>
	void serialize(Archive& archive, glm::vec3& m)
	{
		archive(m.x, m.y, m.z);
	}

	template<class Archive>
	void serialize(Archive& archive, glm::vec4& m)
	{
		archive(m.x, m.y, m.z, m.w);
	}
}