#pragma once

#ifdef __swift__
#include <stdint.h>
#else
#include <cstdint>
#endif

#include "Common/Macros.hxx"

SWIFT_EXPORTED(numCores())
uint8_t NumCores();
