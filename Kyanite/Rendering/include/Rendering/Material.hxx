#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include "ShaderBinding.hxx"
#include "TextureBuffer.hxx"


namespace Renderer {
    struct Material {
        std::shared_ptr<GraphicsShaderBinding> Shader;
        std::vector<std::shared_ptr<TextureBuffer>> Textures;
    };
}