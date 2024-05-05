#!/bin/bash

generator=$(wc -l < $1)
vcpkg=$(wc -l < $2)

# Step 1: Put the bridging headers into scripting/bridge
cp engine/assetpackages/include/assetpackages/Bridge_AssetPackages.h scripting/bridge/include/Bridge_AssetPackages.h
cp engine/audio/include/audio/Bridge_Audio.h scripting/bridge/include/Bridge_Audio.h
cp engine/core/include/core/Bridge_Core.h scripting/bridge/include/Bridge_Core.h
cp engine/ecs/include/ecs/Bridge_ECS.h scripting/bridge/include/Bridge_ECS.h
cp engine/input/include/input/Bridge_Input.h scripting/bridge/include/Bridge_Input.h
cp engine/rendering/include/rendering/Bridge_Rendering.h scripting/bridge/include/Bridge_Rendering.h
cp engine/shared/include/shared/Exported.hxx scripting/bridge/include/Exported.hxx
cp engine/shared/include/shared/NativePointer.hxx scripting/bridge/include/NativePointer.h

# Step 2: Build the lower level libraries via cmake. Use first command for vcpkg dir, second for generator and third for qtdir.
cmake -G $generator -B build -S . -DCMAKE_TOOLCHAIN_FILE=$vcpkg

# Step 3: Build the swift package
swift build -Xlinker .\build\engine\assetpackages\Debug\AssetPackages.lib -Xlinker .\build\engine\audio\Debug\Audio.lib -Xlinker .\build\engine\core\Debug\Core.lib -Xlinker .\build\engine\ecs\Debug\EntityComponentSystem.lib -Xlinker .\build\engine\input\Debug\Input.lib -Xlinker .\build\engine\rendering\Debug\Rendering.lib

