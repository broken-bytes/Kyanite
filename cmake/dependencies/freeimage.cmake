SET(
    FreeImageIncDir
    ${CMAKE_SOURCE_DIR}/vendor/FreeImage/include
)

if(WIN32)
SET(
    FreeImageBinDir
    ${CMAKE_SOURCE_DIR}/vendor/FreeImage/bin/win64
)
SET(
    FreeImageLibDir
    ${CMAKE_SOURCE_DIR}/vendor/FreeImage/lib/win64
)
endif()