include(FetchContent)  

if(WIN32)
FetchContent_Declare(
    DxHeaders 
    GIT_REPOSITORY "https://github.com/microsoft/DirectX-Headers.git"         
    GIT_TAG "9ca4839a1b49aeac56c86036212dc035b1cf4a09"
) 

FetchContent_MakeAvailable(DxHeaders)  

endif()