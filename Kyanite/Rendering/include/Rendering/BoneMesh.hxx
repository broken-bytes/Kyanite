#pragma once

#include "./Mesh.hxx"

struct Bone {
    float* Positions;
    float* Rotations;
    float* Scales;
    int NumPositions;
    int NumRotations;
    int NumScalings;
    int ID;
    void* LocalTransformation;
} typedef Bone;

struct BoneMesh {
    Mesh* Skin;
    Bone* Bones;
} typedef BoneMesh;