include(FetchContent)  

FetchContent_Declare(
    Flecs       
    GIT_REPOSITORY "https://github.com/SanderMertens/flecs.git"  
    GIT_TAG "5fc3e45f5eceb9fcad3acba619f3a04208318de1" 
)  

FetchContent_MakeAvailable(Flecs)  