#include "d3d12/D3D12TextureBuffer.hxx"

#include <string>
#include "d3d12/Utils.hxx"

namespace Renderer {
	D3D12TextureBuffer::D3D12TextureBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> buffer, std::string name) {
		_buffer = buffer;
		std::wstring str;
		Utils::ConvertStrToWstr(str, name);
		buffer->SetName(str.c_str());

	}
}