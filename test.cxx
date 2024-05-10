#include <editor/assetpipeline/include/assetpipeline/Bridge_AssetPipeline.h>
#include <engine/io/include/io/Bridge_IO.h>

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <cstdlib> // For _putenv_s function
#include <filesystem>

int main() {
	std::string path = "Box.fbx";
	MeshData* meshdata = nullptr;
	size_t len;
	auto result = AssetPipeline_LoadMeshes(path.c_str(), &meshdata, &len);
	if (result.error != nullptr) {
		std::cout << "Failed to load mesh" << std::endl;
		return 1;
	}
	else {
		std::cout << "Loaded mesh" << std::endl;
	}
}