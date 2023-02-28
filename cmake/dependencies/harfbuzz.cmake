include(FetchContent)  

FetchContent_Declare(
    HarfBuzz       
    GIT_REPOSITORY "https://github.com/harfbuzz/harfbuzz.git"  
    GIT_TAG "1d1f93a612ed071b703abab7deb0951f46a12433" 
)  

FetchContent_MakeAvailable(HarfBuzz)  