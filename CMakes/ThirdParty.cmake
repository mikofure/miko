include(FetchContent)
include(ExternalProject)

# Function to auto-build dependencies from GitHub
function(miko_fetch_dependency name git_repo git_tag)
    string(TOLOWER ${name} name_lower)
    
    if(MIKO_AUTO_BUILD_DEPS)
        message(STATUS "Fetching ${name} from ${git_repo}")
        
        FetchContent_Declare(
            ${name_lower}
            GIT_REPOSITORY ${git_repo}
            GIT_TAG ${git_tag}
            GIT_SHALLOW TRUE
        )
        
        FetchContent_MakeAvailable(${name_lower})
        set(${name}_FOUND TRUE PARENT_SCOPE)
    endif()
endfunction()

# Auto-fetch common dependencies if not found
if(MIKO_AUTO_BUILD_DEPS)
    # SDL2
    find_package(SDL2 QUIET)
    if(NOT SDL2_FOUND)
        miko_fetch_dependency(SDL2 "https://github.com/libsdl-org/SDL.git" "release-2.28.5")
    endif()
    
    # nlohmann_json
    find_package(nlohmann_json QUIET)
    if(NOT nlohmann_json_FOUND)
        miko_fetch_dependency(nlohmann_json "https://github.com/nlohmann/json.git" "v3.11.3")
    endif()
    
    # GLEW (if using OpenGL)
    if(NOT MIKO_USE_VULKAN)
        find_package(GLEW QUIET)
        if(NOT GLEW_FOUND)
            miko_fetch_dependency(GLEW "https://github.com/nigels-com/glew.git" "glew-2.2.0")
        endif()
    endif()
    
    # Google Test (if building tests)
    if(MIKO_BUILD_TESTS)
        find_package(GTest QUIET)
        if(NOT GTest_FOUND)
            miko_fetch_dependency(GTest "https://github.com/google/googletest.git" "v1.14.0")
        endif()
    endif()
endif()