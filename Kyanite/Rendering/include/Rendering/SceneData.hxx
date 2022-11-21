#pragma once 

#include "../../math/Vec4.hxx"
#include "../../math/Mat4.hxx"

namespace Renderer {
	struct SceneData {
		Math::Mat4<float> Model;
		Math::Mat4<float> View;
		Math::Mat4<float> Projection;
		Math::Vec4<float> AmbientColor;
	};
}