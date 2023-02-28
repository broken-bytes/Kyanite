include(FetchContent)  

FetchContent_Declare(
    Zlib       
    GIT_REPOSITORY "https://github.com/intel/zlib.git"  
    GIT_TAG "d9c2508ff9493514b1ef9851eb9623a7fae3a4c0" 
)  

FetchContent_MakeAvailable(Zlib)  