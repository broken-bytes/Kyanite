#pragma once

#include "Resource.hxx"

namespace Renderer {
	class RenderTarget: public Resource {
	public:
		virtual auto Reset() -> void = 0;
	};
}