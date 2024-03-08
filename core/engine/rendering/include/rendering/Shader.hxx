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
    struct ShaderData {
        std::string name;
        std::string source;
        ShaderType type;

        ShaderData() = default;
        ShaderData(
            const std::string name, 
            const std::string source, 
            ShaderType type
        ) : name(name), source(source), type(type) {}

    private:
        friend class cereal::access;

        template <class Archive>
        void serialize(Archive& Data) {
            Data(name, source);
        }
    };

	class Shader {
    public:
        std::string name;
        ShaderType type;
        uint64_t id;

        Shader() = default;
        Shader(const std::string name, ShaderType type, uint64_t id) : name(name), type(type), id(id) {}
	};
}