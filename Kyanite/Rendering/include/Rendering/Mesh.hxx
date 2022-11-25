// #SwiftImport
#pragma once

#include <cstddef>
#include <cstdint>
#include "Vertex.hxx"

#ifdef __cplusplus 
extern "C" {
#endif

enum MeshDrawInfoFlags {
    MESH_DRAW_INFO_FLAGS_DRAW_NONE = 0,
    MESH_DRAW_INFO_FLAGS_DRAW_OUTLINE = 1
};


struct MeshDrawInfo {
    uint8_t Flags;
    float OutlineWidth;
    float OutlineColor[4];
} typedef MeshDrawInfo;

struct Mesh {
   Vertex* Vertices;
   size_t NumVertices;
   uint32_t* Indices;
   size_t NumIndices;
} typedef Mesh;

#ifdef __cplusplus 
}
#endif