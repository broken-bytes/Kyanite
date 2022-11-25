#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "ShaderBinding.hxx"
#include "TextureBuffer.hxx"


namespace Renderer {
    struct MaterialProp {
        ShaderPropType Type;
    };

    template<typename T>
    struct MaterialPropT: public MaterialProp {
        T Value;
    };

    struct MaterialFloatProp: public MaterialPropT<float> {
        MaterialFloatProp() {
            this->Type = ShaderPropType::FLOAT;
        }
    };
    struct MaterialFloat2Prop: public MaterialPropT<std::array<float, 2>> {
        MaterialFloat2Prop() {
            this->Type = ShaderPropType::FLOAT2;
        }
    };
    struct MaterialFloat3Prop: public MaterialPropT<std::array<float, 3>> {
        MaterialFloat3Prop() {
            this->Type = ShaderPropType::FLOAT3;
        }
    };
    struct MaterialFloat4Prop: public MaterialPropT<std::array<float, 4>> {
        MaterialFloat4Prop() {
            this->Type = ShaderPropType::FLOAT4;
        }
    };
    struct MaterialBoolProp: public MaterialPropT<bool> {
        MaterialBoolProp() {
            this->Type = ShaderPropType::BOOL;
        }
    };
    struct MaterialIntProp: public MaterialPropT<int> {
         MaterialIntProp() {
            this->Type = ShaderPropType::INT;
        }
    };
    struct Material {
        std::shared_ptr<GraphicsShaderBinding> Shader;
        std::map<std::string, std::shared_ptr<MaterialProp>> Props;
        std::map<std::string, std::shared_ptr<TextureBuffer>> Textures;
    };
}