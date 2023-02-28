include(FetchContent)  

FetchContent_Declare(
    Freetype       
    GIT_REPOSITORY "https://github.com/freetype/freetype.git"  
    GIT_TAG "de8b92dd7ec634e9e2b25ef534c54a3537555c11" 
)  

FetchContent_MakeAvailable(Freetype)  