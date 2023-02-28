#pragma once

#include "Buffer.hxx"
#include "Color.hxx"
#include "DrawCall.hxx"
#include "GraphicsRootSignature.hxx"
#include "Resource.hxx"
#include "ResourceState.hxx"

#include <vector>

namespace Rendering {

    class CommandList {
        
        public:
        enum class CommandListType {

            DRAW,
            UPLOAD,
            COPY,
            COMPUTE
        };
        
        virtual auto ClearColor() -> void = 0;
        virtual auto ClearDepth() -> void = 0;
        virtual auto ClearRenderTarget(Resource target, Color color) -> void = 0;
        virtual auto Close() -> void = 0;
        virtual auto Copy(uint32_t startX, uint32_t startY, uint32_t width, uint32_t height, Resource src, Resource dst) -> void = 0;
        virtual auto Draw(Buffer vertexBuffer, Buffer indexBuffer, uint64_t instances, Buffer instanceData) -> void = 0;
        virtual auto Reset() -> void = 0;
        virtual auto SetGraphicsRootSignature(GraphicsRootSignature signature) -> void = 0;
        virtual auto SetDescriptorHeaps(std::vector<Buffer> heaps) -> void = 0;
        virtual auto SetGraphicsRootConstantBuffer(Buffer heap, void* data, size_t sizeInBytes, uint8_t* address) -> void = 0;
        virtual auto SetGraphicsRootDescriptorTable(uint32_t index, Resource resource) -> void = 0;
        virtual auto Transition(Resource resource, ResourceState from, ResourceState to) -> void = 0;
        virtual auto UpdateSubresources(Buffer src, Buffer dst, void* data, size_t rowPitch, size_t slicePitch) -> void = 0;
    };
}