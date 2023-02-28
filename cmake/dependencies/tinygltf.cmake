
include(FetchContent)  

FetchContent_Declare(
    TinyGltf       
    GIT_REPOSITORY "https://github.com/syoyo/tinygltf.git"  
    GIT_TAG "84a83d39f55dcd12d722b77c5181349f4c840f5b" 
)  

FetchContent_MakeAvailable(TinyGltf)  