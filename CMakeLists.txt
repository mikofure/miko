cmake_minimum_required(VERSION 3.25)

project(Miko
    VERSION 0.1.0
    DESCRIPTION "Miko GPU-accelerated UI Framework (SDL2-based, Windows/Linux)"
    LANGUAGES CXX C
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include modular CMake files
include(CMakes/Options.cmake)
include(CMakes/Platform.cmake)
include(CMakes/ThirdParty.cmake)
include(CMakes/Dependencies.cmake)
include(CMakes/Sources.cmake)
include(CMakes/Target.cmake)
include(CMakes/Examples.cmake)
include(CMakes/Installation.cmake)
include(CMakes/Utils.cmake)