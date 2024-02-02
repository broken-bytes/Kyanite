#include "rendering/Bridge_Rendering.h"
#include "rendering/Rendering.hxx"

namespace rendering = kyanite::engine::rendering;

void Rendering_Init(NativePointer window) {
	rendering::Init(window);
}

void Rendering_Update(float deltaTime) {
	rendering::Update(deltaTime);
}

void Rendering_Render() {
	rendering::Render();
}
