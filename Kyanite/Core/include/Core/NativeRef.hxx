// #SwiftImport
#pragma once

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif 
enum RefType {
    STATIC,
    DYNAMIC,
};

struct NativeRef {
    const char* UUID;
    uint16_t RefCount;
    uint64_t Identifier;
    RefType Type;
    // Function pointer to function called upon delete of this Pointer
    void* Deleter;

} typedef NativeRef;

#ifdef __cplusplus
}
#endif