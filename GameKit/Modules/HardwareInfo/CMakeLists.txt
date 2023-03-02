add_library(HardwareInfo STATIC src/HardwareInfo.cxx)

target_include_directories(HardwareInfo PUBLIC include)
target_include_directories(HardwareInfo PRIVATE ${CMAKE_SOURCE_DIR}/GameKit/Shared/include)