#pragma once

#include <string>
#include <vector>

namespace Renderer {
    struct ShaderData {
        std::string ShaderCode;
        std::vector<std::string> Properties;
    };
}
