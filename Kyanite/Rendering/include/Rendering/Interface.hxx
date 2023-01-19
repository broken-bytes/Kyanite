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
#include "Mesh.hxx"


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
		auto MidFrame() -> void;
		auto EndFrame() -> void;
		auto Resize(std::uint32_t width, std::uint32_t height) -> void;
		auto CreateMaterial(std::string name, uint64_t shaderId) -> uint64_t;
		auto MaterialPushTexture(uint64_t textureId) -> void;
		auto MaterialPopTexture() -> void;
		auto SetMaterialTexture(uint64_t material, std::string name, uint64_t texture) -> void;
		auto SetMaterialFloat(uint64_t material, std::string name, float value) -> void;
		auto SetMaterialVector2(uint64_t material, std::string name, float* value) -> void;
		auto SetMaterialVector3(uint64_t material, std::string name, float* value) -> void;
		auto SetMaterialVector4(uint64_t material, std::string name, float* value) -> void;
		auto SetMaterialInt(uint64_t material, std::string name, int value) -> void;
		auto SetMaterialBool(uint64_t material, std::string name, bool value) -> void;
		auto UploadMeshData(Vertex* vertices, size_t vCount, Index* indices, size_t iCount) -> std::uint64_t;
		auto UploadTextureData(std::uint8_t* data, std::uint16_t width, std::uint16_t height, std::uint8_t channels) -> std::uint64_t;
		auto UploadShaderData(GraphicsShader shader)->std::uint64_t;
		auto DrawMesh(uint64_t entityId, uint64_t meshId, uint64_t materialId, MeshDrawInfo info,
                         glm::vec3 position, glm::quat rotation, glm::vec3 scale) -> void;
		auto SetMeshProperties() -> void;
		auto MeshDataFor(const char* uuid) -> std::vector<std::uint64_t>;
		auto SetCamera(glm::vec3 position, glm::vec3 rotation) -> void;
		auto SetCursorPosition(std::array<uint32_t, 2> position) -> void;
		auto ReadMouseOverData(uint32_t x, uint32_t y) -> uint32_t;
		auto GetOutputTexture() -> uint64_t;
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
		std::shared_ptr<Allocator> _mouseOverAllocator;

		std::shared_ptr<GraphicsCommandList> _mouseOverCommandList;
		std::shared_ptr<CommandQueue> _mainQueue;

		std::shared_ptr<GraphicsCommandList> _cbCommandList;
		std::array<std::shared_ptr<Allocator>, FRAME_COUNT> _cbAllocator;


		// Uploading
		std::shared_ptr<GraphicsCommandList> _uploadCommandList;
		std::shared_ptr<Allocator> _uploadAllocator;
		std::shared_ptr<Fence> _uploadFence;
		std::uint64_t _uploadFenceValue;
		void* _uploadFenceEvent;

		//std::shared_ptr<Allocator> _copyAllocator;
		//std::shared_ptr<CommandQueue> _copyQueue;
		//std::shared_ptr<GraphicsCommandList> _copyCommandList;

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
		std::shared_ptr<Buffer> _mouseOverDepthBuffer;
		glm::vec3 cameraRotation = {};
		glm::vec3 cameraMovement = {};
		glm::mat4 _viewMatrix;
		glm::mat4 _projectionMatrix;
		std::uint64_t _resourceCounter;
		std::array<uint32_t, 2> _cursorPosition = {0,0};

		std::vector<std::shared_ptr<UploadBuffer>> _uploadBuffers = {};
		std::map<uint64_t, std::vector<uint8_t>> _materialBuffers;
		std::map<uint64_t, std::shared_ptr<UploadBuffer>> _materialCBVs = {};
		std::array<std::shared_ptr<RenderTarget>, FRAME_COUNT> _mouseOverRTV;
		std::array<std::shared_ptr<TextureBuffer>, FRAME_COUNT> _mouseOverBuffer;
		std::shared_ptr<ReadbackBuffer> _mouseOverReadbackBuffer;
		std::array<uint32_t, 4> _currentEntityColor;
		std::shared_ptr<UploadBuffer> _entitySelectionBuffer;
		uint64_t _entitySelectionCBVId;
		uint64_t _mouseOverRowPitchActual;
  		uint64_t _mouseOverRowPitchRequired; 
  		uint64_t _mouseOverPadding;


		// --- DEFAULT SHADERS & MATERIALS --- 
		// Color Only shader. -> Used for outlining and mouse over detection
		std::shared_ptr<GraphicsShaderBinding> _colorOnlyShader;
		// Storage
		auto CreatePipeline() -> void;
		auto CreateAssets() -> void;
		auto CreateUI(void* context) -> void;
		auto FillCommandList() -> void;
		auto MoveToNextFrame() -> void;
		auto GenerateMipMaps() -> void;
		auto WaitForGPU(std::shared_ptr<CommandQueue> queue) -> void;
		auto FlushGPU() -> void;
    };
}
