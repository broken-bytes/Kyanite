#pragma once

#include <cstdint>
#include <cassert>
#include <stddef.h>
#include <d3d12.h>
#include <wrl.h>
#include "Buffer.hxx"
#include <string>
#include "d3d12/Utils.hxx"

namespace Renderer {
	template<typename T>
	class D3D12Buffer: public Buffer {
	public:
		D3D12Buffer(Microsoft::WRL::ComPtr<ID3D12Resource> buffer, T view, size_t count, std::string name) {
			this->Buffer= buffer;
			this->View = view;
			this->Count = count;
			std::wstring str;
			Utils::ConvertStrToWstr(str, name);
			buffer->SetName(str.c_str());
		}
		virtual ~D3D12Buffer() {};
		Microsoft::WRL::ComPtr<ID3D12Resource> Buffer;
		T View;

		auto Size()->size_t {
			return Count;
		}
	};
}
