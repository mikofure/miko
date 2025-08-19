cmake_minimum_required(VERSION 3.20)
project(miko VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Platform specific settings
if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files - manually specified
set(MIKO_SOURCES
    src/core/Application.cpp
    src/core/Window.cpp
    src/core/Renderer.cpp
    src/platform/Win32Window.cpp
    src/platform/D2DRenderer.cpp
    src/widgets/Widget.cpp
    src/widgets/Button.cpp
    src/widgets/Label.cpp
    src/widgets/TextBox.cpp
    src/widgets/Panel.cpp
    src/layout/Layout.cpp
    src/layout/StackLayout.cpp
    src/layout/GridLayout.cpp
    src/utils/Math.cpp
    src/utils/Color.cpp
    src/utils/Event.cpp
    src/miko.cpp
)

set(MIKO_HEADERS
    include/miko/miko.h
    include/miko/core/Application.h
    include/miko/core/Window.h
    include/miko/core/Renderer.h
    include/miko/platform/Win32Window.h
    include/miko/platform/D2DRenderer.h
    include/miko/widgets/Widget.h
    include/miko/widgets/Button.h
    include/miko/widgets/Label.h
    include/miko/widgets/TextBox.h
    include/miko/widgets/Panel.h
    include/miko/layout/Layout.h
    include/miko/layout/StackLayout.h
    include/miko/layout/GridLayout.h
    include/miko/utils/Math.h
    include/miko/utils/Color.h
    include/miko/utils/Event.h
)

# Create the miko library
add_library(miko STATIC ${MIKO_SOURCES} ${MIKO_HEADERS})

# Link libraries
if(WIN32)
    target_link_libraries(miko 
        d2d1
        dwrite
        dwmapi
        user32
        gdi32
        kernel32
        ole32
        oleaut32
        uuid
        comctl32
    )
endif()

# Set target properties
set_target_properties(miko PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Example application
add_subdirectory(examples)

# Install targets
install(TARGETS miko
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)