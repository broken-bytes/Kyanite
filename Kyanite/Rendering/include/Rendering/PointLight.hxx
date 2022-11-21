#pragma once
#include "../../math/Vec3.hxx"

namespace Renderer {
    struct PointLight {
        Math::Vec3<float> Position;
        Math::Vec3<float> Color;
        float Radius;
        float Intensity;
    };
}