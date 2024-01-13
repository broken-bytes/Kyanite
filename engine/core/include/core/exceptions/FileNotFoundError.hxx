#pragma once

#include <exception>
#include <stdexcept>

struct FileNotFoundError: std::runtime_error {
    FileNotFoundError(std::string file) : std::runtime_error(file) { }
};
