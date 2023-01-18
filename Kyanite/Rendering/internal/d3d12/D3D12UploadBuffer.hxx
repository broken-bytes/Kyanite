#pragma once 

#include "UploadBuffer.hxx"
#include <d3d12.h>
#include <wrl.h>
#include "d3d12/Utils.hxx"

namespace Renderer {
	class D3D12UploadBuffer: public UploadBuffer {
	public:
		D3D12UploadBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> buffer, std::string name) {
			Buffer = buffer;
			std::wstring str;
			Utils::ConvertStrToWstr(str, name);
			buffer->SetName(str.c_str());
		}

		Microsoft::WRL::ComPtr<ID3D12Resource> Buffer;
	};
}