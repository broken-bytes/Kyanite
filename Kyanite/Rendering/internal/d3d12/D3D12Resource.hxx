#pragma once

#include "Resource.hxx"

namespace Renderer {
	class D3D12Resource: public Resource {
	public:
		virtual auto Resource()->ID3D12Resource* = 0;
	};
}