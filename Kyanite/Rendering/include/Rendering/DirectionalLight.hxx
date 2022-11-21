#pragma once

#include "../../math/Vec3.hxx"

namespace Renderer {
    struct DirectionalLight {
        Math::Vec3<float> Color;
        Math::Vec3<float> Rotation;
        float Intensity;
    };
}

