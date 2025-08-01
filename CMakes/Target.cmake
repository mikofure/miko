# Create main library
add_library(miko STATIC
    ${MIKO_CORE_SOURCES}
    ${MIKO_PLATFORM_SDL2_SOURCES}
    ${MIKO_RENDER_COMMON_SOURCES}
    ${MIKO_TEXT_SOURCES}
    ${MIKO_LAYOUT_SOURCES}
    ${MIKO_UTIL_SOURCES}
    ${MIKO_ASSETS_SOURCES}
    
    # Platform-specific implementations
    $<$<STREQUAL:${MIKO_PLATFORM},windows>:${MIKO_PLATFORM_WINDOWS_SOURCES}>
    $<$<STREQUAL:${MIKO_PLATFORM},linux>:${MIKO_PLATFORM_LINUX_SOURCES}>
    
    # Rendering backends
    $<$<STREQUAL:${MIKO_GRAPHICS_API},vulkan>:${MIKO_RENDER_VULKAN_SOURCES}>
    $<$<STREQUAL:${MIKO_GRAPHICS_API},opengl>:${MIKO_RENDER_OPENGL_SOURCES}>
    
    # Optional features
    $<$<BOOL:${MIKO_ENABLE_SCREEN_CAPTURE}>:
        ${MIKO_SCREEN_CAPTURE_SOURCES}
        $<$<STREQUAL:${MIKO_PLATFORM},windows>:${MIKO_SCREEN_CAPTURE_WINDOWS_SOURCES}>
        $<$<STREQUAL:${MIKO_PLATFORM},linux>:${MIKO_SCREEN_CAPTURE_LINUX_SOURCES}>
    >
    
    $<$<BOOL:${MIKO_ENABLE_HTTP_CLIENT}>:${MIKO_HTTP_SOURCES}>
    $<$<BOOL:${MIKO_ENABLE_INSPECTOR}>:${MIKO_INSPECTOR_SOURCES}>
)

# Include directories
target_include_directories(miko
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${SDL2_INCLUDE_DIRS}
    PRIVATE
        src
        $<$<STREQUAL:${MIKO_GRAPHICS_API},vulkan>:${Vulkan_INCLUDE_DIRS}>
)

# Compile definitions
target_compile_definitions(miko
    PRIVATE
        MIKO_VERSION="${PROJECT_VERSION}"
        MIKO_SDL2_BACKEND
        $<$<CONFIG:Debug>:MIKO_DEBUG>
        $<$<BOOL:${MIKO_ENABLE_INSPECTOR}>:MIKO_INSPECTOR>
        $<$<BOOL:${MIKO_ENABLE_LEAK_DETECTION}>:MIKO_LEAK_DETECTION>
        $<$<BOOL:${MIKO_RUNTIME_SHADERS}>:MIKO_RUNTIME_SHADERS>
        $<$<BOOL:${MIKO_ENABLE_SCREEN_CAPTURE}>:MIKO_SCREEN_CAPTURE>
        $<$<STREQUAL:${MIKO_GRAPHICS_API},vulkan>:MIKO_VULKAN>
        $<$<STREQUAL:${MIKO_GRAPHICS_API},opengl>:MIKO_OPENGL>
        $<$<PLATFORM_ID:Windows>:MIKO_WINDOWS>
        $<$<PLATFORM_ID:Linux>:MIKO_LINUX>
        # SDL2 platform defines
        $<$<PLATFORM_ID:Windows>:SDL_MAIN_HANDLED>
)

# Link libraries
target_link_libraries(miko
    PUBLIC
        Threads::Threads
        ${SDL2_LIBRARIES}
        nlohmann_json::nlohmann_json
        PNG::PNG
        JPEG::JPEG

    PRIVATE
        # Graphics API libraries
        $<$<STREQUAL:${MIKO_GRAPHICS_API},vulkan>:${Vulkan_LIBRARIES}>
        $<$<STREQUAL:${MIKO_GRAPHICS_API},opengl>:
            ${OPENGL_LIBRARIES}
            GLEW::GLEW
        >

        # Platform-specific libraries
        $<$<PLATFORM_ID:Windows>:
            ${DWRITE_LIB}
            ${D2D1_LIB}
            winmm
            ole32
            oleaut32
            imm32
            version
            setupapi
            $<$<STREQUAL:${MIKO_GRAPHICS_API},opengl>:${OPENGL32_LIB}>
        >

        $<$<PLATFORM_ID:Linux>:
            ${FONTCONFIG_LIBRARIES}
            ${FREETYPE_LIBRARIES}
            dl
            $<$<BOOL:${MIKO_ENABLE_SCREEN_CAPTURE}>:
                ${X11_LIBRARIES}
                ${XRANDR_LIBRARIES}
            >
        >

        # Optional libraries
        $<$<BOOL:${MIKO_ENABLE_HTTP_CLIENT}>:CURL::libcurl>
)

# Compiler-specific flags
target_compile_options(miko
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:
            -Wall -Wextra -Wpedantic -Wno-unused-parameter
        >
        $<$<CXX_COMPILER_ID:MSVC>:
            /W4 /wd4100 /wd4201 /wd4996
        >
        $<$<CONFIG:Debug>:
            $<$<CXX_COMPILER_ID:GNU,Clang>:-g -O0>
            $<$<CXX_COMPILER_ID:MSVC>:/Od /Zi>
        >
        $<$<CONFIG:Release>:
            $<$<CXX_COMPILER_ID:GNU,Clang>:-O3 -DNDEBUG -flto>
            $<$<CXX_COMPILER_ID:MSVC>:/O2 /DNDEBUG /GL>
        >
)

# Link-time optimization for release builds
set_target_properties(miko PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
)