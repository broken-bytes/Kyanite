#pragma once

#include "Device.hxx"

namespace Renderer {
struct Viewport {
    float TopLeftX = 0;
    float TopLeftY = 0;
    float Width = 0;
    float Height = 0;
    float MinDepth = 0;
    float MaxDepth = 10000;
    Viewport() {
        TopLeftX = 0;
        TopLeftY = 0;
        Width = 0;
        Height = 0;
        MinDepth = 0;
        MaxDepth = 10000;
    }
    Viewport(
         float topLeftX,
         float topLeftY,
         float width,
         float height,
         float minDepth = 0,
         float maxDepth = 10000)
{
    this->TopLeftX = topLeftX;
    this->TopLeftY = topLeftY;
    this->Width = width;
    this->Height = height;
    this->MinDepth = minDepth;
    this->MaxDepth = maxDepth;
}
};
}
