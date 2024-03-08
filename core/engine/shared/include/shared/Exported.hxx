#pragma once

#include <stdint.h>

#define SWIFT_EXPORTED(x) __attribute__((swift_name(#x)))
#define SWIFT_STRUCTED __attribute__((swift_wrapper(struct)));
#define SWIFT_PRIVATE __attribute__((swift_private));

#if _WIN32
#define EXPORTED __declspec(dllexport)
#endif