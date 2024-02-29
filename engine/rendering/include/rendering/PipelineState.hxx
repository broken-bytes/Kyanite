#pragma once

#include "PrimitiveTopology.hxx"
#include "Shader.hxx"

#include <memory>

namespace kyanite::engine::rendering {
	class PipelineState {
	public:

		virtual ~PipelineState() = default;
	};
}