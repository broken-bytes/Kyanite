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
EXPORTED void Rendering_Init(NativePointer window, NativePointer imGuiContext);

/**
 * @brief Shuts down the rendering system
 *
 */
EXPORTED void Rendering_Shutdown();

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
    @brief Starts the rendering frame
*/
EXPORTED void Rendering_PreFrame();

/**
 * @brief Updates the rendering system
 * @param deltaTime The time since the last frame
 *
 */
EXPORTED void Rendering_Update(float deltaTime);

/**
* @brief Render the frame
*/
EXPORTED void Rendering_PostFrame();

// Graphics pipeline functions

/**
* @brief Creates a vertex buffer
* @param vertices The vertices to use
* @param length The length of the buffer in count of vertices
* @return The id of the vertex buffer
*/
EXPORTED uint64_t Rendering_CreateVertexBuffer(NativePointer vertices, size_t length);

/**
* @brief Updates a vertex buffer
* @param vertexBufferId The id of the vertex buffer to update
* @param vertices The new vertices to use
*/
EXPORTED void Rendering_UpdateVertexBuffer(uint64_t vertexBufferId, NativePointer vertices, size_t length);

/**
* @brief Frees a vertex buffer
* @param vertexBufferId The id of the vertex buffer to free
*/
EXPORTED void Rendering_FreeVertexBuffer(uint64_t vertexBufferId);

/**
* @brief Loads the vertex buffer into cpu memory
* @param vertexBufferId The id of the vertex buffer to load
* @param vertices The pointer to the cpu memory
* @param length The length of the buffer in count of vertices
*/
EXPORTED void Rendering_LoadVertexBuffer(uint64_t vertexBufferId, NativePointer* vertices, size_t* length);

/**
* @brief Creates an index buffer
* @param indices The indices to use
* @param length The length of the buffer in count of indices
* @return The id of the index buffer
*/
EXPORTED uint64_t Rendering_CreateIndexBuffer(NativePointer indices, size_t length);

/**
* @brief Updates an index buffer
* @param indexBufferId The id of the index buffer to update
* @param indices The new indices to use
*/
EXPORTED void Rendering_UpdateIndexBuffer(uint64_t indexBufferId, NativePointer indices);

/**
* @brief Frees an index buffer
* @param indexBufferId The id of the index buffer to free
*/
EXPORTED void Rendering_FreeIndexBuffer(uint64_t indexBufferId);

/**
* @brief Creates a shader
* @param shader The shader code to use
* @param shaderType The type of the shader. 0 for vertex, 1 for fragment
* @return The id of the shader
*/
EXPORTED uint64_t Rendering_CreateShader(const char* shader, uint8_t shaderType);

/**
* @brief Destroys a shader
* @param shaderId The id of the shader to destroy
*/
EXPORTED void Rendering_DestroyShader(uint64_t shaderId);

#ifdef __cplusplus 
}
#endif
