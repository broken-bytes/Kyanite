#include "d3d12/D3D12ReadbackBuffer.hxx"

#include <algorithm>
#include <vector>
#include <wrl.h>

namespace Renderer {
D3D12ReadbackBuffer::D3D12ReadbackBuffer(
    Microsoft::WRL::ComPtr<ID3D12Resource> buffer, size_t sizeInBytes) {
  _buffer = buffer;
  _sizeInBytes = sizeInBytes;
  buffer->SetName(L"ReadbackBuffer");

  _bufferData = new uint32_t[sizeInBytes / 4];
  _tempBuffer = new uint8_t[sizeInBytes];
}

auto D3D12ReadbackBuffer::ReadBytes()
    -> uint32_t* {
  D3D12_RANGE readbackBufferRange{0, _sizeInBytes};
  _buffer->Map(0, &readbackBufferRange,
               reinterpret_cast<void **>(&_tempBuffer));

  std::copy(_tempBuffer, _tempBuffer + _sizeInBytes, (uint8_t*)_bufferData);


  D3D12_RANGE emptyRange{0, 0};
  _buffer->Unmap(0, &emptyRange);

  return _bufferData;
}
} // namespace Renderer