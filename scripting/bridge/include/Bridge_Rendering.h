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

// ImGUI functions

/**
 * @brief Starts a new window
 * @param name The name of the window
 * @param p_open A pointer to a boolean that indicates if the window is open
 */
EXPORTED void Rendering_StartWindow(const char* name, bool* p_open);

/**
 * @brief Ends the current window
 */
EXPORTED void Rendering_EndWindow();

/**
    @brief Draws some text
*/
EXPORTED void Rendering_DrawText(const char* text);

/**
 * @brief Draws a button
 * @param label The label of the button
 * @return True if the button was clicked, false otherwise
 */
EXPORTED bool Rendering_DrawButton(const char* label);

/**
 * @brief Draws a slider
 * @param label The label of the slider
 * @param value The value of the slider
 * @param min The minimum value of the slider
 * @param max The maximum value of the slider
 * @return The new value of the slider
 */
EXPORTED float Rendering_DrawSlider(const char* label, float value, float min, float max);

/**
 * @brief Draws a color picker
 * @param label The label of the color picker
 * @param color The color to pick
 */
EXPORTED void Rendering_DrawColorPicker(const char* label, float* color);

/**
 * @brief Draws a 2D vector
 * @param label The label of the vector
 * @param values The values of the vector
 */
EXPORTED void Rendering_DrawVector2(const char* label, float* values);

/**
 * @brief Draws a 3D vector
 * @param label The label of the vector
 * @param values The values of the vector
 */
EXPORTED void Rendering_DrawVector3(const char* label, float* values);

/**
 * @brief Draws a 4D vector
 * @param label The label of the vector
 * @param values The values of the vector
 */
EXPORTED void Rendering_DrawVector4(const char* label, float* values);

/**
 * @brief Draws a combo box
 * @param label The label of the combo box
 * @param currentItem The current item
 * @param items The items of the combo box
 * @param itemsCount The number of items
 * @return The new current item
 */
EXPORTED int Rendering_DrawComboBox(const char* label, int currentItem, const char** items, int itemsCount);

/**
 * @brief Draws an image
 * @param textureId The id of the texture to draw
 * @param width The width of the image
 * @param height The height of the image
 */
EXPORTED void Rendering_DrawImage(uint64_t textureId, uint32_t width, uint32_t height);

// Draw a drag and drop target in imgui with the given label and target
EXPORTED void Rendering_DrawDragAndDropTarget(const char* label, void* target);

#ifdef __cplusplus 
}
#endif
