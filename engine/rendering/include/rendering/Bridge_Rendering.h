#pragma once

#include <shared/Exported.hxx>
#include <shared/NativePointer.hxx>

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus 
extern "C" {
#endif

/**
 * @brief Inits the rendering system
 *
 */
EXPORTED void Rendering_Init(NativePointer window);

/**
 * @brief Sets the clear color of the rendering system
 * @param r The red component of the color
 * @param g The green component of the color
 * @param b The blue component of the color
 * @param a The alpha component of the color
 *
 */
EXPORTED void Rendering_SetClearColor(float r, float g, float b, float a);

/**
* @brief Sets the viewport of the rendering system
* @param x The x coordinate of the viewport
* @param y The y coordinate of the viewport
* @param width The width of the viewport
* @param height The height of the viewport
*/
EXPORTED void Rendering_SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

/**
 * @brief Draws a mesh
 * @param shaderId The id of the shader to use
 * @param vertexBuffer The vertex buffer to use
 * @param indexBuffer The index buffer to use
 * @param vertexCount The number of vertices to draw
 * @param indexCount The number of indices to draw
 */
EXPORTED void Rendering_DrawMesh(uint64_t shaderId, uint32_t vertexBuffer, uint32_t indexBuffer, uint32_t vertexCount, uint32_t indexCount);


/**
 * @brief Updates the rendering system
 * @param deltaTime The time since the last frame
 *
 */
EXPORTED void Rendering_Update(float deltaTime);

/**
* @brief Lets the renderer draw all the queued commands
*/
EXPORTED void Rendering_Render();

#ifdef __cplusplus 
}
#endif
