@echo off

:: Step 1: Put the bridging headers into ./scripting/bridge
robocopy "core/editor/assetdatabase/include/assetdatabase" "scripting/bridge/include" "Bridge_AssetDatabase.h"
robocopy "core/editor/assetpipeline/include/assetpipeline" "scripting/bridge/include" "Bridge_AssetPipeline.h"
robocopy "core/engine/audio/include/audio" "scripting/bridge/include" "Bridge_Audio.h"
robocopy "core/engine/ecs/include/ecs" "scripting/bridge/include/" "Bridge_ECS.h"
robocopy "core/engine/imgui/include/imgui" "scripting/bridge/include" "Bridge_ImGui.h"
robocopy "core/engine/input/include/input" "scripting/bridge/include" "Bridge_Input.h"
robocopy "core/engine/io/include/io" "scripting/bridge/include" "Bridge_IO.h"
robocopy "core/engine/rendering/include/rendering" "scripting/bridge/include" "Bridge_Rendering.h"
robocopy "core/engine/system/include/system" "scripting/bridge/include" "Bridge_System.h"
robocopy "core/engine/shared/include/shared" "scripting/bridge/include/shared" "Events.hxx"
robocopy "core/engine/shared/include/shared" "scripting/bridge/include/shared" "Exported.hxx"
robocopy "core/engine/shared/include/shared" "scripting/bridge/include/shared" "NativePointer.hxx"
robocopy "core/engine/shared/include/shared" "scripting/bridge/include/shared" "Result.hxx"

:: Step 2: Build the lower level libraries via cmake. Use first command for vcpkg dir, second for generator and third for qtdir.
cmake -G %1 -B ./build -S . -DCMAKE_TOOLCHAIN_FILE=%2

if %1 == "Ninja" (
    ninja -C build
) else (
    msbuild build/Kyanite.sln
)

:: Step 3: Build the swift package
swift build -Xlinker lib\Debug\AssetDatabase.lib -Xlinker lib\Debug\AssetPipeline.lib -Xlinker lib\Debug\Audio.lib -Xlinker lib\Debug\EntityComponentSystem.lib -Xlinker lib\Debug\ImGUI.lib -Xlinker lib\Debug\Input.lib -Xlinker lib\Debug\IO.lib -Xlinker lib\Debug\System.lib -Xlinker lib\Debug\Rendering.lib

:: Copy all C++ dlls to the swift package directory
robocopy "bin/Debug" "./.build/x86_64-unknown-windows-msvc/debug" "*.dll"
:: Also copy all pdb files
robocopy "bin/Debug" "./.build/x86_64-unknown-windows-msvc/debug" "*.pdb"
