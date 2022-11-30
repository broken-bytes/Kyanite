#include <algorithm>
#include <dxgiformat.h>
#include <memory>
#include "Renderer.hxx"
#include "d3d12/D3D12ReadbackBuffer.hxx"
#include "glm/glm.hpp"
#include "Rect.hxx"
#include "d3d12/D3D12Allocator.hxx"
#include "d3d12/D3D12GraphicsCommandList.hxx"
#include "d3d12/D3D12GraphicsPipelineState.hxx"
#include "d3d12/D3D12GraphicsRootSignature.hxx"
#include "d3d12/D3D12Heap.hxx"
#include "d3d12/D3D12RenderTarget.hxx"
#include "Resource.hxx"
#include "d3d12/D3D12Resource.hxx"
#include "d3d12/D3D12Buffer.hxx"
#include "d3d12/D3D12UploadBuffer.hxx"
#include "d3d12/D3D12TextureBuffer.hxx"


namespace Renderer {
	D3D12GraphicsCommandList::D3D12GraphicsCommandList(CommandType type, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList5> commandList): GraphicsCommandList(type) {
		_commandList = commandList;
	}

	auto D3D12GraphicsCommandList::Close() -> void {
		_commandList->Close();
	}

	auto D3D12GraphicsCommandList::Reset(std::shared_ptr<Allocator> allocator, std::shared_ptr<GraphicsPipelineState> pipelineState) -> void {
		auto pipeline = static_pointer_cast<D3D12GraphicsPipelineState>(pipelineState);
		_commandList->Reset(static_pointer_cast<D3D12Allocator>(allocator)->Native(), pipeline? pipeline-> Native() : nullptr);
	}

	auto D3D12GraphicsCommandList::ClearRenderTarget(std::shared_ptr<DescriptorHandle> target, glm::vec4 color) -> void {
		float arr[] = {color.x, color.y, color.z, color.w};
		_commandList->ClearRenderTargetView(
			static_pointer_cast<DescriptorHandleT<D3D12_CPU_DESCRIPTOR_HANDLE>>(target)->Handle(),
			arr,
			0,
			nullptr
		);
	}

	auto D3D12GraphicsCommandList::ClearDepthTarget(std::shared_ptr<DescriptorHandle> target, float depth) -> void {
		_commandList->ClearDepthStencilView(
			static_pointer_cast<DescriptorHandleT<D3D12_CPU_DESCRIPTOR_HANDLE>>(target)->Handle(),
			D3D12_CLEAR_FLAG_DEPTH,
			depth,
			0,
			0,
			nullptr
		);
	}

	auto D3D12GraphicsCommandList::SetPipelineState(std::shared_ptr<GraphicsPipelineState> state) -> void {
		_commandList->SetPipelineState(static_pointer_cast<D3D12GraphicsPipelineState>(state)->Native());
	}

	auto D3D12GraphicsCommandList::SetGraphicsRootSignature(std::shared_ptr<GraphicsRootSignature> signature) -> void {
		_commandList->SetGraphicsRootSignature(static_pointer_cast<D3D12GraphicsRootSignature>(signature)->Native());
	}

	auto D3D12GraphicsCommandList::SetDescriptorHeaps(std::vector<std::shared_ptr<Heap>> heaps) -> void {
		std::vector<ID3D12DescriptorHeap*> d3d12Vec;

		for (const auto& item : heaps) {
			d3d12Vec.push_back(static_pointer_cast<D3D12Heap>(item)->Native());
		}
		_commandList->SetDescriptorHeaps(heaps.size(), d3d12Vec.data());
	}

	auto D3D12GraphicsCommandList::SetGraphicsRootConstantBuffer(std::shared_ptr<UploadBuffer> heap, void* data, size_t sizeInBytes, uint8_t* address) -> void{
		CD3DX12_RANGE readRange(0, 0);
		auto dHeap = static_pointer_cast<D3D12UploadBuffer>(heap);
		dHeap->Buffer->Map(0, &readRange, reinterpret_cast<void**>(&address));
		memcpy(address, data, sizeInBytes);
	}
	
	auto D3D12GraphicsCommandList::SetGraphicsRootDescriptorTable(
		std::uint32_t index,
		std::shared_ptr<DescriptorHandle> handle
	) -> void {
		auto dhandle = static_pointer_cast<DescriptorHandleT<D3D12_GPU_DESCRIPTOR_HANDLE>>(handle)->Handle();
		_commandList->SetGraphicsRootDescriptorTable(index, dhandle);
	}
	
	auto D3D12GraphicsCommandList::SetGraphicsRootConstants(std::uint64_t index, std::size_t count, void* data, std::size_t offset) -> void {
		_commandList->SetGraphicsRoot32BitConstants(index, count, data, offset);
	}

	auto D3D12GraphicsCommandList::SetTopology(GraphicsPipelineStateTopology topology) -> void {
		auto topo = D3D12_PRIMITIVE_TOPOLOGY(topology);
		_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY(topology));
	}
	
	auto D3D12GraphicsCommandList::SetViewport(Viewport viewport) -> void {
		D3D12_VIEWPORT vp = {};
		vp.Width = viewport.Width;
		vp.Height = viewport.Height;
		vp.TopLeftX = viewport.TopLeftX;
		vp.TopLeftY = viewport.TopLeftY;
		vp.MaxDepth = viewport.MaxDepth;
		vp.MinDepth = viewport.MinDepth;
		_commandList->RSSetViewports(1, &vp);
	}
	
	auto D3D12GraphicsCommandList::SetScissorRect(Rect rect) -> void {
		D3D12_RECT dr = {};
		dr.left = rect.Left;
		dr.top = rect.Top;
		dr.bottom = rect.Bottom;
		dr.right = rect.Right;
		_commandList->RSSetScissorRects(1, &dr);
	}
	
	auto D3D12GraphicsCommandList::SetRenderTarget(
		std::shared_ptr<DescriptorHandle> rtvHandle,
		std::shared_ptr<DescriptorHandle> dsvHandle
	) -> void {
		auto rHandle = static_pointer_cast<DescriptorHandleT<D3D12_CPU_DESCRIPTOR_HANDLE>>(rtvHandle)->Handle();
		auto dHandle = static_pointer_cast<DescriptorHandleT<D3D12_CPU_DESCRIPTOR_HANDLE>>(dsvHandle)->Handle();
		_commandList->OMSetRenderTargets(
			1,
			&rHandle,
			false,
			&dHandle
		);
	}

	auto D3D12GraphicsCommandList::SetVertexBuffer(
		std::shared_ptr<Buffer> buffer
	) -> void {
		_commandList->IASetVertexBuffers(0, 1, &static_pointer_cast<D3D12Buffer<D3D12_VERTEX_BUFFER_VIEW>>(buffer)->View);
	}

	auto D3D12GraphicsCommandList::SetIndexBuffer(
		std::shared_ptr<Buffer> buffer
	) -> void {
		_commandList->IASetIndexBuffer(&static_pointer_cast<D3D12Buffer<D3D12_INDEX_BUFFER_VIEW>>(buffer)->View);
	}

	auto D3D12GraphicsCommandList::Draw(
		std::uint32_t vertexCount,
		std::uint32_t instanceCount,
		std::uint32_t startVertex,
		std::uint32_t startInstance) -> void {

	}

	auto D3D12GraphicsCommandList::DrawInstanced(
		std::uint32_t indexCount,
		std::uint32_t instanceCount,
		std::uint32_t startIndex,
		std::uint32_t startVertex,
		std::uint32_t startInstance
	) -> void {
		_commandList->DrawIndexedInstanced(indexCount, instanceCount, startIndex, startVertex, startInstance);
	}
	
	auto D3D12GraphicsCommandList::Transition(std::shared_ptr<Resource> resource, ResourceState from, ResourceState to) -> void {
		auto res1 = static_pointer_cast<D3D12Resource>(resource)->Resource();
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			static_pointer_cast<D3D12Resource>(resource)->Resource(),
			D3D12_RESOURCE_STATES(from),
			D3D12_RESOURCE_STATES(to)
		);
		_commandList->ResourceBarrier(1, &barrier);
	}

	auto D3D12GraphicsCommandList::Transition(std::shared_ptr<RenderTarget> resource, ResourceState from, ResourceState to) -> void {
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			static_pointer_cast<D3D12RenderTarget>(resource)->Resource(),
			D3D12_RESOURCE_STATES(from),
			D3D12_RESOURCE_STATES(to)
		);
		_commandList->ResourceBarrier(1, &barrier);
	}

	auto D3D12GraphicsCommandList::Transition(Resource* resource, ResourceState from, ResourceState to) -> void {
		//auto rt = static_pointer_cast<D3D12RenderTarget>(renderTarget)->RenderTarget();
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			reinterpret_cast<D3D12Resource*>(resource)->Resource(),
			D3D12_RESOURCE_STATES(from),
			D3D12_RESOURCE_STATES(to)
		);
		_commandList->ResourceBarrier(1, &barrier);
	}

	auto D3D12GraphicsCommandList::Transition(void* resource, ResourceState from, ResourceState to) -> void {
		//auto rt = static_pointer_cast<D3D12RenderTarget>(renderTarget)->RenderTarget();
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			reinterpret_cast<ID3D12Resource*>(resource),
			D3D12_RESOURCE_STATES(from),
			D3D12_RESOURCE_STATES(to)
		);
		_commandList->ResourceBarrier(1, &barrier);
	}

	auto D3D12GraphicsCommandList::Transition(std::shared_ptr<Renderer::Buffer> buffer, ResourceState from, ResourceState to) -> void {
		//auto rt = static_pointer_cast<D3D12RenderTarget>(renderTarget)->RenderTarget();
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			static_pointer_cast<D3D12Buffer<int>>(buffer)->Buffer.Get(),
			D3D12_RESOURCE_STATES(from),
			D3D12_RESOURCE_STATES(to)
		);
		_commandList->ResourceBarrier(1, &barrier);
	}

	auto D3D12GraphicsCommandList::Transition(std::shared_ptr<Renderer::TextureBuffer> buffer, ResourceState from, ResourceState to) -> void {
		//auto rt = static_pointer_cast<D3D12RenderTarget>(renderTarget)->RenderTarget();
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			static_pointer_cast<D3D12TextureBuffer>(buffer)->Raw(),
			D3D12_RESOURCE_STATES(from),
			D3D12_RESOURCE_STATES(to)
		);
		_commandList->ResourceBarrier(1, &barrier);
	}

	auto D3D12GraphicsCommandList::Copy(std::shared_ptr<RenderTarget> from, std::shared_ptr<ReadbackBuffer> to) -> void {
		auto renderTarget = static_pointer_cast<D3D12RenderTarget>(from);
		auto readbackBuffer = static_pointer_cast<D3D12ReadbackBuffer>(to);

		_commandList->CopyResource(renderTarget->Resource(), readbackBuffer->Raw());
	}

	auto D3D12GraphicsCommandList::Copy(uint32_t startX, uint32_t startY, uint32_t width, uint32_t height, std::shared_ptr<TextureBuffer> from, std::shared_ptr<ReadbackBuffer> to) -> void {
		auto texture = static_pointer_cast<D3D12TextureBuffer>(from);
		auto readbackBuffer = static_pointer_cast<D3D12ReadbackBuffer>(to);


        D3D12_PLACED_SUBRESOURCE_FOOTPRINT bufferFootprint = {};
        bufferFootprint.Footprint.Width = width;
        bufferFootprint.Footprint.Height = height;
        bufferFootprint.Footprint.Depth = 1;
        bufferFootprint.Footprint.RowPitch = width * 4;
        bufferFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		const CD3DX12_TEXTURE_COPY_LOCATION copyDest(readbackBuffer->Raw(), bufferFootprint);
        const CD3DX12_TEXTURE_COPY_LOCATION copySrc(texture->Raw(), 0);

		Transition(from, ResourceState::RENDER_TARGET, ResourceState::COPY_SOURCE);

		_commandList->CopyTextureRegion(&copyDest, 0, 0, 0, &copySrc, nullptr);

		Transition(from, ResourceState::COPY_SOURCE, ResourceState::RENDER_TARGET);
	}

	auto D3D12GraphicsCommandList::Native() const->ID3D12GraphicsCommandList5* {
		return _commandList.Get();
	}
	auto D3D12GraphicsCommandList::UpdateSubresources(std::shared_ptr<Buffer> dst, std::shared_ptr<UploadBuffer> src, void* data, size_t rowPitch, size_t slicePitch) -> void {
		D3D12_SUBRESOURCE_DATA subData = {};
		subData.pData = data;
		subData.RowPitch = rowPitch;
		subData.SlicePitch = slicePitch;

		::UpdateSubresources(_commandList.Get(), static_pointer_cast<D3D12Buffer<int8_t>>(dst)->Buffer.Get(), static_pointer_cast<D3D12UploadBuffer>(src)->Buffer.Get(), 0, 0, 1, &subData);
	}

	auto D3D12GraphicsCommandList::UpdateSubresources(std::shared_ptr<TextureBuffer> dst, std::shared_ptr<UploadBuffer> src, void* data, size_t rowPitch, size_t slicePitch) -> void {
		D3D12_SUBRESOURCE_DATA subData = {};
		subData.pData = data;
		subData.RowPitch = rowPitch;
		subData.SlicePitch = slicePitch;

		::UpdateSubresources(_commandList.Get(), static_pointer_cast<D3D12TextureBuffer>(dst)->Raw(), static_pointer_cast<D3D12UploadBuffer>(src)->Buffer.Get(), 0, 0, 1, &subData);
	}

}