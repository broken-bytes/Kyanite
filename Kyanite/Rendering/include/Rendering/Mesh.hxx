// #SwiftImport
#pragma once

#include <cstddef>
#include <cstdint>
#include "Vertex.hxx"

#ifdef __cplusplus 
extern "C" {
#endif

struct Mesh {
   Vertex* Vertices;
   size_t NumVertices;
   uint32_t* Indices;
   size_t NumIndices;
} typedef Mesh;

#ifdef __cplusplus 
}
#endif