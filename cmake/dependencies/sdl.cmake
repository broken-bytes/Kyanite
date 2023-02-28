include(FetchContent)  

FetchContent_Declare(
    SDL2       
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"  
    GIT_TAG "adf31f6ec0be0f9ba562889398f71172c7941023" 
)  

FetchContent_MakeAvailable(SDL2)  