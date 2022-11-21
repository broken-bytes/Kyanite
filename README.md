# Kyanite

## Building
The dependencies are not included in the project, therefore it is up to the user to supply these.

For ease of use, I recommend vcpkg
Install dependencies via vcpg:
- freeimage
- sdl2
- tinygltf

Add the vcpkg toolchain to CMAKE e.g. -DCMAKE_TOOLCHAIN_FILE=C:/libs/vcpkg/scripts/buildsystems/vcpkg.cmake
