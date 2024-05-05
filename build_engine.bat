@echo off

:: Step 1: Put the bridging headers into ./scripting/bridge
Xcopy "./core/editor/assetdatabase/include/assetdatabase/Bridge_AssetDatabase.h" "./scripting/bridge/include/Bridge_AssetDatabase.h"
Xcopy "./core/editor/assetpipeline/include/assetpipeline/Bridge_AssetPipeline.h" "./scripting/bridge/include/Bridge_AssetPipeline.h"
Xcopy "./core/engine/audio/include/audio/Bridge_Audio.h" "./scripting/bridge/include/Bridge_Audio.h"
Xcopy "./core/engine/ecs/include/ecs/Bridge_ECS.h" "./scripting/bridge/include/Bridge_ECS.h"
Xcopy "./core/engine/imgui/include/imgui/Bridge_ImGui.h" "./scripting/bridge/include/Bridge_ImGui.h"
Xcopy "./core/engine/input/include/input/Bridge_Input.h" "./scripting/bridge/include/Bridge_Input.h"
Xcopy "./core/engine/io/include/io/Bridge_IO.h" "./scripting/bridge/include/Bridge_IO.h"
Xcopy "./core/engine/rendering/include/rendering/Bridge_Rendering.h" "./scripting/bridge/include/Bridge_Rendering.h"
Xcopy "./core/engine/system/include/system/Bridge_System.h" "./scripting/bridge/include/Bridge_System.h"
Xcopy "./core/engine/shared/include/shared/Events.hxx" "./scripting/bridge/include/shared/Events.h*"
Xcopy "./core/engine/shared/include/shared/Exported.hxx" "./scripting/bridge/include/shared/Exported.h*"
Xcopy "./core/engine/shared/include/shared/NativePointer.hxx" "./scripting/bridge/include/shared/NativePointer.h*"

:: Step 2: Build the lower level libraries via cmake. Use first command for vcpkg dir, second for generator and third for qtdir.
cmake -G %1 -B ./build -S . -DCMAKE_TOOLCHAIN_FILE=%2

if %1 == "Ninja" (
    ninja -C build
) else (
    msbuild build/Kyanite.sln
)

:: Step 3: Build the swift package
swift build -Xlinker lib\Debug\AssetDatabase.lib -Xlinker lib\Debug\AssetPipeline.lib -Xlinker lib\Debug\Audio.lib -Xlinker lib\Debug\EntityComponentSystem.lib -Xlinker lib\Debug\ImGUI.lib -Xlinker lib\Debug\Input.lib -Xlinker lib\Debug\IO.lib -Xlinker lib\Debug\System.lib -Xlinker lib\Debug\Rendering.lib

