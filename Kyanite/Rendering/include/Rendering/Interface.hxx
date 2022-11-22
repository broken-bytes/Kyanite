#pragma once

#include <array>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Material.hxx"
#include "glm/glm.hpp"
#include "Allocator.hxx"
#include "Buffer.hxx"
#include "CommandQueue.hxx"
#include "Device.hxx"
#include "DrawCall.hxx"
#include "Fence.hxx"
#include "Frame.hxx"
#include "GraphicsCommandList.hxx"
#include "GraphicsPipelineState.hxx"
#include "GraphicsRootSignature.hxx"
#include "Heap.hxx"
#include "IndexBuffer.hxx"
#include "Renderer.hxx"
#include "Rect.hxx"
#include "Shader.hxx"
#include "SwapChain.hxx"
#include "TextureBuffer.hxx"
#include "Vertex.hxx"
#include "VertexBuffer.hxx"
#include "VertexArrayObject.hxx"
#include "Viewport.hxx"
#include "ShaderBinding.hxx"


namespace Renderer {
    constexpr uint8_t FRAME_COUNT = 2;
	
	typedef uint32_t Index;
	enum RenderBackendAPI {
		DirectX12 = 0,
		OpenGL = 1
	};
    class Interface {
        public: 
        Interface(std::uint32_t width, std::uint32_t height, void* window, RenderBackendAPI type);
        ~Interface();
		float yOffset = 0;

		auto StartFrame() -> void;
		auto EndFrame() -> void;
		auto Update() -> void;
		auto Resize(std::uint32_t width, std::uint32_t height) -> void;
		auto CreateMaterial(uint64_t shaderId, std::vector<uint64_t> textureIds) -> uint64_t;
		auto UploadMeshData(Vertex* vertices, size_t vCount, Index* indices, size_t iCount) -> std::uint64_t;
		auto UploadTextureData(std::uint8_t* data, std::uint16_t width, std::uint16_t height, std::uint8_t channels) -> std::uint64_t;
		auto UploadShaderData(const char* data)->std::uint64_t;
		auto DrawMesh(uint64_t id, uint64_t materialId, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) -> void;
		auto SetMeshProperties() -> void;
		auto MeshDataFor(const char* uuid) -> std::vector<std::uint64_t>;
		auto RotateCamera(float rotationX, float rotationY, float rotationZ) -> void;
		auto MoveCamera(float moveX, float moveY, float moveZ) -> void;

	protected:
		void* _window;
		void* _context;
		std::shared_ptr<Device> _device;
		glm::vec2 _windowDimension;
		std::unordered_map<const char*, std::vector<std::uint64_t>> _uploadedMeshes = {};
        std::shared_ptr<SwapChain> _swapChain;
		Viewport _viewport = {};
		Rect _scissorRect;
		std::shared_ptr<Fence> _fence;
		std::array<std::shared_ptr<Frame>, FRAME_COUNT> _frames;
		std::vector<std::shared_ptr<GraphicsRootSignature>> _rootSignatures = {};

		// Heaps
		std::shared_ptr<Heap> _rtvHeap;
		std::shared_ptr<Heap> _samplerHeap;
		std::shared_ptr<Heap> _cbvHeap;
		std::shared_ptr<Heap> _srvHeap;
		uint64_t _srvCounter = 0;
		std::shared_ptr<Heap> _uavHeap;
		std::shared_ptr<Heap> _dsvHeap;

		std::shared_ptr<GraphicsPipelineState> _defaultPipeline;
		std::shared_ptr<GraphicsCommandList> _commandList;
		std::shared_ptr<CommandQueue> _mainQueue;

		std::shared_ptr<GraphicsCommandList> _cbCommandList;
		std::array<std::shared_ptr<Allocator>, FRAME_COUNT> _cbAllocator;


		// Uploading
		std::shared_ptr<GraphicsCommandList> _uploadCommandList;
		std::shared_ptr<Allocator> _uploadAllocator;
		std::shared_ptr<Fence> _uploadFence;
		std::uint64_t _uploadFenceValue;
		void* _uploadFenceEvent;

		std::shared_ptr<Allocator> _computeAllocator;
		std::shared_ptr<CommandQueue> _computeQueue;
		std::shared_ptr<GraphicsCommandList> _computeCommandList;
		std::shared_ptr<Fence> _computeFence;
		std::uint64_t _computeFenceValue;
		void* _computeFenceEvent;

		std::array<uint8_t*, FRAME_COUNT> _lightDataGPUAddress;
		std::array<std::shared_ptr<Heap>, FRAME_COUNT> _lightDataHeap;
		std::array<std::shared_ptr<UploadBuffer>, FRAME_COUNT> _lightDataBuffer;




		// Frame 
		std::uint32_t _frameIndex;
		std::uint32_t _nextFrameIndex;
		void* _fenceEvent;

		// Rendering
		std::vector<std::shared_ptr<VertexArrayObject>> _meshes = {};
		std::vector<DrawCall> _meshesToDraw = {};
		std::vector < std::shared_ptr<TextureBuffer>> _textures;
		std::vector<std::shared_ptr<GraphicsShaderBinding>> _shaders;
		std::vector<std::shared_ptr<Material>> _materials;
		std::vector <std::shared_ptr<Buffer>> _constantBuffers = {};
		std::shared_ptr<Buffer> _depthBuffer;
		glm::vec3 cameraRotation = {};
		glm::vec3 cameraMovement = {};
		glm::mat4 _viewMatrix;
		glm::mat4 _projectionMatrix;
		std::uint64_t _resourceCounter;

		std::vector<std::shared_ptr<UploadBuffer>> _uploadBuffers = {};

		// Storage
		auto CreatePipeline() -> void;
		auto CreateAssets() -> void;
		auto CreateUI(void* context) -> void;
		auto FillCommandList() -> void;
		auto MoveToNextFrame() -> void;
		auto GenerateMipMaps() -> void;
		auto WaitForGPU(std::shared_ptr<CommandQueue> queue) -> void;

    };
}
