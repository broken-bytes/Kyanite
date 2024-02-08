#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>

#include <string>
#include <vector>


namespace kyanite::engine::rendering {
    enum class ShaderType {
		VERTEX,
		FRAGMENT,
		GEOMETRY,
		TESS_CONTROL,
		TESS_EVALUATION,
		COMPUTE
	};
	struct Shader {
        std::string name;

        Shader() = default;
        Shader(const std::string name) : name(name) {}

    private:
        friend class cereal::access;

        template <class Archive>
        void serialize(Archive& Data) {
            Data(name);
        }
	};
}