#pragma once

#include "Exported.hxx"
#include <shared/Exported.hxx>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct Result {
    void* data;
    size_t size;
    const char* error;
} typedef Result;

inline struct Result Result_OK() {
    struct Result result;
    result.data = (void*)"OK";
    result.size = 2;
    result.error = NULL;

	return result;
}

inline struct Result Result_Error(const char* error) {
    struct Result result;
    result.data = NULL;
    result.size = 0;
    result.error = error;

    return result;
}