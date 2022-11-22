#include "Interface.hxx"
#include "Vertex.hxx"
#include "Viewport.hxx"
#include "Rect.hxx"

#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec4.hpp"
#include "glm/matrix.hpp"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <SDL_syswm.h>

#ifdef __APPLE__
#include "backends/opengl/OpenGLDevice.hxx"
#endif // __APPLE__

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define NO_MIN_MAX
#include <Windows.h>
#include "d3d12/D3D12Device.hxx"
#endif
#include <d3dx12.h>

#include <clocale>

#include "d3d12/D3D12Heap.hxx"



float timeOfDay = 0;

float xPos = -5;


namespace Renderer
{
	struct MVPCBuffer {
		glm::mat4 Model;
		glm::mat4 ViewProjection;
	};

	struct MaterialBuffer {
		float Specular;
		float Emmisive;
	};

	struct SunLight {
		glm::vec4 Color;
		glm::vec4 Position;
		bool Active;
		uint8_t Padding[15];
	};

	struct PointLight {
		glm::vec4 Color;
		glm::vec4 Position;
		float Radius;
		float Intensity;
		bool Active;
		uint8_t Padding[4];
	};

	struct LightBuffer {
		SunLight Sun;
		glm::vec4 Ambient;
		PointLight Lights[16];
	};

	Interface::Interface(uint32_t width, uint32_t height, void* window, RenderBackendAPI type)
	{
		SDL_SysWMinfo wmInfo;
			SDL_VERSION(&wmInfo.version);
			SDL_GetWindowWMInfo(reinterpret_cast<SDL_Window*>(window), &wmInfo);
			HWND hwnd = wmInfo.info.win.window;
		_windowDimension = { (float)width, (float)height };
		switch (type)
		{
		case DirectX12:
			_device = static_pointer_cast<Device>(std::make_shared<D3D12Device>(hwnd));
			break;
#ifdef __APPLE__
		case OpenGL:
			_device = std::make_shared<OpenGLDevice>(width, height, window);
			break;
#endif // __APPLE__
		default:
		_device = static_pointer_cast<Device>(std::make_shared<D3D12Device>(reinterpret_cast<HWND>(window)));
			break;
		}
		setlocale(LC_ALL, "en_US.utf8");

		CreatePipeline();
	}

	Interface::~Interface()
	{
	}

	auto Interface::StartFrame() -> void
	{
		// Start Projection
		_uploadFenceValue++;
		_uploadCommandList->Close();
		_mainQueue->ExecuteCommandLists({ _uploadCommandList });

		auto _ = _mainQueue->Signal(_uploadFence, _uploadFenceValue);

		_uploadFence->Wait(_uploadFenceValue, _uploadFenceEvent);

		_uploadBuffers.clear();

		_computeFenceValue++;

		_computeCommandList->Close();
		_computeQueue->ExecuteCommandLists({ _computeCommandList });

		_ = _computeQueue->Signal(_computeFence, _computeFenceValue);

		_uploadFence->Wait(_computeFenceValue, _computeFenceEvent);


		auto fenceValue = _frames[_frameIndex]->FenceValue();

		_frames[_frameIndex]->Allocator()->Reset();
		_commandList->Reset(_frames[_frameIndex]->Allocator(), nullptr);
		_cbAllocator[_frameIndex]->Reset();
		_cbCommandList->Reset(_cbAllocator[_frameIndex], nullptr);


		auto rtvHandle = _rtvHeap->CpuHandleFor(_frameIndex);
		auto dsvHandle = _dsvHeap->CpuHandleFor(0);
		_commandList->SetRenderTarget(rtvHandle, dsvHandle);

		timeOfDay += 0.01f;

		if (timeOfDay > 24) {
			timeOfDay = 0;
		}


		xPos = (timeOfDay - 12);

		_commandList->SetDescriptorHeaps({ _srvHeap });
		auto light = LightBuffer{};

		auto sun = SunLight{
			{ 0.4f, 0.4f, 0.4f,1 },
			{ xPos, cos(timeOfDay) , 3, 1},
			true
		};

		light.Ambient = { 1, 1, 1, 1};

		light.Sun = sun;

		light.Lights[0] = {
			{0, 0, 1, 1},
			{0, -5.5f, 4, 1},
			20,
			5,
			true
		};

		light.Lights[1] = {
			{1, 0, 1, 1},
			{4, 3, 4, 1},
			10,
			8,
			true
		};
		_commandList->SetGraphicsRootConstantBuffer(_lightDataBuffer[_frameIndex], &light, sizeof(light), _lightDataGPUAddress[_frameIndex]);


		_commandList->Transition(
			_frames[_frameIndex]->RenderTarget(),
			ResourceState::PRESENT,
			ResourceState::RENDER_TARGET
		);

		// Record commands.
		glm::vec4 clearColor = { 0, 0.1f, 0.4f, 1 };
		_commandList->ClearRenderTarget(rtvHandle, clearColor);
		_commandList->ClearDepthTarget(dsvHandle);
		_commandList->SetTopology(GraphicsPipelineStateTopology::POLYGON);
		_commandList->SetViewport(_viewport);
		_commandList->SetScissorRect(_scissorRect);


		glm::vec3 cameraPos = { 0, 0, -20 };
		glm::vec3 cameraFront = { 0.0f, 0.0f, 1.0f };
		glm::vec3 cameraLookAt{ 0, 0, 0 };
		glm::vec3 cameraUp = { 0.0f, 1.0f, 0.0f };

		_viewMatrix = glm::lookAtLH(cameraPos, cameraLookAt, cameraUp);
		//_viewMatrix = glm::inverse(_viewMatrix);
		_projectionMatrix = glm::perspectiveFovLH(glm::radians(55.0f), (float)_windowDimension.x, (float)_windowDimension.y, 0.01f, 1000.0f);

		for (auto& mesh : _meshesToDraw)
		{
			// Update the projection matrix.
			float aspectRatio = _windowDimension.x / static_cast<float>(_windowDimension.y);
			// Update the MVP matrix


			glm::mat4 modelView = mesh.Transform;
			modelView = glm::scale(modelView, { 1, 1, 1 });
			modelView = glm::translate(modelView, { 0, -6.0f, 4 });
			modelView = glm::rotate(modelView, glm::radians(-130.0f), glm::vec3{ 1.0f, 0.0f, 0.0f });


			yOffset -= 0.001f;

			if (yOffset < -50) {
				yOffset = 50;
			}
			//


			auto mvp = MVPCBuffer{
				modelView,
				_projectionMatrix * _projectionMatrix
			};


			auto shader = std::find_if(_shaders.begin(), _shaders.end(), [mesh](auto e) { return e->ShaderIndex == mesh.ShaderId; });

			auto mat = MaterialBuffer{};
			mat.Emmisive = 0.1f;
			mat.Specular = 0.5f;


			if (shader != _shaders.end()) {
				_commandList->SetGraphicsRootSignature(shader->get()->Pipeline->RootSignature());
				_commandList->SetPipelineState(shader->get()->Pipeline);
				_commandList->SetGraphicsRootConstants(0, sizeof(mvp) / 4, &mvp, 0);
				//_commandList->SetGraphicsRootConstants(4, sizeof(LightBuffer) / 4, &light, 0);
				_commandList->SetGraphicsRootDescriptorTable(1, _textures[0]->GPUHandle);
				_commandList->SetGraphicsRootDescriptorTable(2, _textures[0]->GPUHandle);
				_commandList->SetGraphicsRootConstants(3, sizeof(MaterialBuffer) / 4, &mat, 0);		
				_commandList->SetGraphicsRootDescriptorTable(4, _srvHeap->GpuHandleFor(_frameIndex));


				// 2x Table = 2 | 2
				// 2x SRV = 4   | 4 + 2 = 6
				// 
			}
			auto len = sizeof(float);

			auto vao = std::find_if(_meshes.begin(), _meshes.end(), [mesh](std::shared_ptr<VertexArrayObject> vao) { return vao->Index == mesh.MeshId; });
			if (vao != _meshes.end()) {
				_commandList->SetVertexBuffer(vao->get()->VertexBuffer);
				_commandList->SetIndexBuffer(vao->get()->IndexBuffer);
				_commandList->DrawInstanced(vao->get()->IndexBuffer->Size(), 1, 0, 0, 0);
			}
		}

		// Indicate that the back buffer will now be used to present.
		_commandList->Transition(
			_frames[_frameIndex]->RenderTarget(),
			ResourceState::RENDER_TARGET,
			ResourceState::PRESENT
		);

		_commandList->Close();
		_cbCommandList->Close();

		_mainQueue->ExecuteCommandLists({ _commandList  });

		// Present the frame.
		_swapChain->Swap();
		_meshesToDraw.clear();
	}

	auto Interface::EndFrame() -> void
	{
		MoveToNextFrame();
		_device->Flush();
		_uploadAllocator->Reset();
		_uploadCommandList->Reset(_uploadAllocator, nullptr);
		_computeAllocator->Reset();
		_computeCommandList->Reset(_computeAllocator, nullptr);
	}

	auto Interface::Update() -> void
	{
	}

	auto Interface::Resize(uint32_t width, uint32_t height) -> void
	{
	}

	auto Interface::UploadMeshData(Vertex* vertices, size_t vCount, Index* indices, size_t iCount) -> uint64_t
	{
		std::vector<Vertex> vertBuffer = {};

		// 9 floats = 1 vert
		for (int x = 0; x < vCount; x += 9) {
			Vertex v {};
			std::memcpy(v.Position, vertices[x].Position, 4 * sizeof(float));
			std::memcpy(v.Normal, vertices[x].Normal, 3 * sizeof(float));
			std::memcpy(v.UV, vertices[x].UV, 2 * sizeof(float));
			vertBuffer.push_back(v);
		}


		std::vector<std::uint32_t> indBuffer = {};

		for (int x = 0; x < iCount; x++) {
			indBuffer.push_back(indices[x]);
		}

		size_t vertSize = vertBuffer.size() * sizeof(Vertex);
		auto vertUpload = _device->CreateUploadBuffer(vertSize);
		size_t indSize = indBuffer.size() * sizeof(uint32_t);
		auto indUpload = _device->CreateUploadBuffer(indSize);

		_uploadBuffers.push_back(vertUpload);
		_uploadBuffers.push_back(indUpload);

		auto vao = std::make_shared<VertexArrayObject>(_resourceCounter, _device->CreateVertexBuffer(vertBuffer), _device->CreateIndexBuffer(indBuffer));
		_uploadCommandList->UpdateSubresources(vao->VertexBuffer, vertUpload, vertices, vertBuffer.size() * sizeof(Vertex), 1);
		_uploadCommandList->Transition(
			static_pointer_cast<Buffer>(vao->VertexBuffer),
			ResourceState::COPY_DEST,
			ResourceState::VERTEX_CONST_BUFFER
		);

		_uploadCommandList->UpdateSubresources(vao->IndexBuffer, indUpload, indices, iCount * sizeof(uint32_t), 1);
		_uploadCommandList->Transition(
			static_pointer_cast<Buffer>(vao->IndexBuffer),
			ResourceState::COPY_DEST,
			ResourceState::VERTEX_CONST_BUFFER
		);

		_meshes.emplace_back(vao);

		return _resourceCounter++;
	}

	auto Interface::UploadTextureData(uint8_t* data, uint16_t width, uint16_t height, uint8_t channels) -> uint64_t
	{
		auto texBuff = _device->CreateTextureBuffer(width, height, channels, 0, Renderer::RGBA);
		auto uploadBuffer = _device->CreateUploadBuffer(width * height * channels);


		_uploadCommandList->Transition(static_pointer_cast<TextureBuffer>(texBuff), ResourceState::COMMON, ResourceState::COPY_DEST);

		_uploadCommandList->UpdateSubresources(texBuff, uploadBuffer, data, width * channels, width * channels * height);

		_device->CreateShaderResourceView(texBuff, _srvHeap->CpuHandleFor(_srvCounter));
		texBuff->GPUHandle = _srvHeap->GpuHandleFor(_srvCounter);
		_uploadCommandList->Transition(
			static_pointer_cast<TextureBuffer>(texBuff),
			ResourceState::COPY_DEST,
			ResourceState::PIXEL_SHADER
		);

		_srvCounter++;
		//_device->CreateMipMaps(texBuff, _computeCommandList, width, height, width / 256);


		_textures.push_back(texBuff);
		_uploadBuffers.push_back(uploadBuffer);

		return _resourceCounter++;
	}

	auto Interface::UploadShaderData(const char* data) -> uint64_t
	{
		auto compiledShader = _device->CompileShader(data);
		compiledShader->ShaderIndex = _shaders.size();
		GraphicsRootSignatureDescription desc = {};
		desc.Parameters = {};
                // MVP Matrix

                auto sizes = {sizeof(MVPCBuffer) / 4,
                              sizeof(MaterialBuffer) / 4,
                              sizeof(LightBuffer) / 4};

                // MVP
                GraphicsRootSignatureParameter param = {
                    0,
                    sizeof(MVPCBuffer) / 4,
                    0,
                    0,
                    GraphicsShaderVisibility::VERTEX,
                    GraphicsRootSignatureParameterType::CONSTANT,
                    GraphicsDescriptorRangeType::CBV};
                desc.Parameters.push_back(param);
                // Diffuse
                param = {1,
                         1,
                         0,
                         0,
                         GraphicsShaderVisibility::PIXEL,
                         GraphicsRootSignatureParameterType::TABLE,
                         GraphicsDescriptorRangeType::SRV};
                desc.Parameters.push_back(param);
                // Normal
                param = {2,
                         1,
                         1,
                         0,
                         GraphicsShaderVisibility::PIXEL,
                         GraphicsRootSignatureParameterType::TABLE,
                         GraphicsDescriptorRangeType::SRV};
                desc.Parameters.push_back(param);
                // Material
                param = {2,
                         1,
                         1,
                         0,
                         GraphicsShaderVisibility::PIXEL,
                         GraphicsRootSignatureParameterType::TABLE,
                         GraphicsDescriptorRangeType::SRV};
                desc.Parameters.push_back(param);
                // Lights
                param = {3,
                         sizeof(MaterialBuffer) / 4,
                         2,
                         0,
                         GraphicsShaderVisibility::VERTEX,
                         GraphicsRootSignatureParameterType::CONSTANT,
                         GraphicsDescriptorRangeType::CBV};
                desc.Parameters.push_back(param);

                auto signature = _device->CreateGraphicsRootSignature(desc);

		std::vector<GraphicsPipelineInputElement> inputElements = {
			{ "POSITION", 0, GraphicsPipelineFormat::RGBA32_FLOAT, 0, 0, GraphicsPipelineClassification::VERTEX, 0 },
			{ "NORMAL", 0, GraphicsPipelineFormat::RGB32_FLOAT, 0, 16, GraphicsPipelineClassification::VERTEX, 0 },
			{ "TEXCOORD", 0, GraphicsPipelineFormat::RG32_FLOAT, 0, 28, GraphicsPipelineClassification::VERTEX, 0 }
		};

		auto pipeline = _device->CreatePipelineState(signature, inputElements, compiledShader);
		compiledShader->Pipeline = pipeline;

		_shaders.push_back(compiledShader);

		return _resourceCounter++;
	}

	auto Interface::DrawMesh(uint64_t id, uint64_t shaderId, uint64_t textureId, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) -> void
	{

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(
			transform, position
		);


		_meshesToDraw.push_back({ id, shaderId, textureId, transform });
	}

	auto Interface::SetMeshProperties() -> void
	{
	}

	auto Interface::MeshDataFor(const char* uuid) -> std::vector<uint64_t>
	{
		return {};
	}

	auto Interface::CreatePipeline() -> void {
		_resourceCounter = 0;
		_mainQueue = _device->CreateCommandQueue(DIRECT);

		_computeQueue = _device->CreateCommandQueue(COMPUTE);
		_computeAllocator = _device->CreateCommandAllocator(COMPUTE);
		_computeCommandList = _device->CreateCommandList(COMPUTE, _computeAllocator, "Compute Queue");

		_swapChain = _device->CreateSwapChain(
			_mainQueue,
			FRAME_COUNT,
			_windowDimension.x,
			_windowDimension.y
		);
		_frameIndex = _swapChain->CurrentBackBufferIndex();

		_rtvHeap = _device->CreateRenderTargetHeap(FRAME_COUNT);
		_srvHeap = _device->CreateShaderResourceHeap(128);
		_uavHeap = _device->CreateUnorderedAccessHeap(100);
		_cbvHeap = _device->CreateConstantBufferHeap(128);
		_dsvHeap = _device->CreateDepthStencilHeap(1);
		_samplerHeap = _device->CreateSamplerHeap(128);
		_meshesToDraw = {};

		_depthBuffer = _device->CreateDepthBuffer(_windowDimension);

		_device->CreateDepthStencilView(
			_dsvHeap,
			_depthBuffer
		);

		for (uint32_t x = 0; x < FRAME_COUNT; x++)
		{
			auto rt = _swapChain->GetBuffer(x);
			auto rtHandle = _rtvHeap->CpuHandleFor(x);
			_device->CreateRenderTarget(rtHandle, rt);
			auto alloc = _device->CreateCommandAllocator(DIRECT);
			_frames[x] = _device->CreateFrame(alloc, rt);
		}

		_scissorRect = { 0u, 0u, static_cast<uint32_t>(_windowDimension.x), static_cast<uint32_t>(_windowDimension.y) };
		_viewport = {
			0.0f,
			0.0f,
			static_cast<float>(_windowDimension.x),
			static_cast<float>(_windowDimension.y),
			0.0f,
			1.0f
		};

		_fence = _device->CreateFence(_frames[0]->FenceValue());
		_uploadFence = _device->CreateFence(_uploadFenceValue);

		_uploadAllocator = _device->CreateCommandAllocator(DIRECT);
		_commandList = _device->CreateCommandList(DIRECT, _frames[_frameIndex]->Allocator(), "CommandList");
		_uploadCommandList = _device->CreateCommandList(DIRECT, _uploadAllocator, "UploadCommandList");

		_commandList->Close();
		_uploadCommandList->Close();
		auto commandLists = { _commandList };
		_mainQueue->ExecuteCommandLists(commandLists);

		_frames[_frameIndex]->IncrementFenceValue();

		// Create an event handle to use for frame synchronization.
		_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (_fenceEvent == nullptr)
		{
			HRESULT_FROM_WIN32(GetLastError());
		}

		_uploadFenceValue = 0;
		_uploadFence = _device->CreateFence(_uploadFenceValue);
#if _WIN32
		_uploadFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (_uploadFenceEvent == nullptr)
		{
			HRESULT_FROM_WIN32(GetLastError());
		}
#endif

		_uploadAllocator->Reset();
		_uploadCommandList->Reset(_uploadAllocator, nullptr);


		_computeFenceValue = 0;
		_computeFence = _device->CreateFence(_computeFenceValue);
#if _WIN32
		_computeFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (_computeFenceEvent == nullptr)
		{
			HRESULT_FROM_WIN32(GetLastError());
		}
#endif

		_computeCommandList->Close();

		_computeAllocator->Reset();
		_computeCommandList->Reset(_computeAllocator, nullptr);



		for (uint32_t x = 0; x < FRAME_COUNT; x++)
		{
			_cbAllocator[x] = _device->CreateCommandAllocator(DIRECT);
			_lightDataBuffer[x] = _device->CreateUploadBuffer((sizeof(LightBuffer) + 255) & ~255);
			_cbAllocator[x]->Reset();

			_device->CreateConstantBufferView(_srvHeap, _lightDataBuffer[x], _srvHeap->CpuHandleFor(_srvCounter++));
		}



		_cbCommandList = _device->CreateCommandList(DIRECT, _cbAllocator[_frameIndex], "Constant Buffer CommandList");
		_cbCommandList->Close();


		WaitForGPU(_mainQueue);
		WaitForGPU(_computeQueue);

	}

	auto Interface::RotateCamera(float rotationX, float rotationY, float rotationZ) -> void {
		//cameraRotation = { rotationX, rotationY, rotationZ };
	}

	auto Interface::MoveCamera(float moveX, float moveY, float moveZ) -> void {
		//cameraMovement = { moveX, moveY, moveZ };
	}

	auto Interface::CreateAssets() -> void
	{
	}


	auto Interface::CreateUI(void* context) -> void
	{
	}

	auto Interface::FillCommandList() -> void
	{
	}

	auto Interface::MoveToNextFrame() -> void
	{
		const uint64_t currentFenceValue = _frames[_frameIndex]->FenceValue();
		_mainQueue->Signal(_fence, currentFenceValue);

		// Update the frame index.
		_frameIndex = _swapChain->CurrentBackBufferIndex();

		auto target = _frames[_frameIndex]->FenceValue();
		// If the next frame is not ready to be rendered yet, wait until it is ready.
		_fence->Wait(_frames[_frameIndex]->FenceValue(), _fenceEvent);

		// Set the fence value for the next frame.
		_frames[_frameIndex]->IncrementFenceValue();
	}

	auto Interface::GenerateMipMaps() -> void
	{
	}

	auto Interface::WaitForGPU(std::shared_ptr<CommandQueue> queue) -> void
	{
		queue->Signal(_fence, _frames[_frameIndex]->FenceValue());

		// Wait until the fence has been processed.
		_fence->SetOnCompletion(_frames[_frameIndex]->FenceValue(), _fenceEvent);
		_fence->Wait(_frames[_frameIndex]->FenceValue(), _fenceEvent);
		// Increment the fence value for the current frame.
		auto _ = _frames[_frameIndex]->IncrementFenceValue();
	}


}


