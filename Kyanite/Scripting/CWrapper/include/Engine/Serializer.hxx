#pragma once

#include <string>
#include "Rendering/Material.hxx"

class Serializer {
    public:
    Serializer(std::string rootPath);

    auto LoadMaterial(std::string path) -> Renderer::Material;
};