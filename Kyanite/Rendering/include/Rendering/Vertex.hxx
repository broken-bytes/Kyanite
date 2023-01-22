// #SwiftImport
#pragma once

#ifdef __cplusplus 
extern "C" {
#endif

struct Vertex {
    float Position[4];
    float Normal[3];
    float UV[2];
    float Color[4];
} typedef Vertex;

struct InstanceVertex {
    float Matrix[16];
} typedef InstanceVertex;

#ifdef __cplusplus 
}
#endif