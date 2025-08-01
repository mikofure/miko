# Find required packages
find_package(Threads REQUIRED)

# SDL2 - Primary windowing and input system
find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 is required but not found. Please install SDL2 development libraries.")
endif()

# Graphics API selection
if(MIKO_USE_VULKAN)
    find_package(Vulkan REQUIRED)
    if(NOT Vulkan_FOUND)
        message(FATAL_ERROR "Vulkan is required but not found. Install Vulkan SDK or disable MIKO_USE_VULKAN.")
    endif()
    set(MIKO_GRAPHICS_API "vulkan")
else()
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    set(MIKO_GRAPHICS_API "opengl")
endif()

# Platform-specific dependencies
if(UNIX AND NOT APPLE)
    # Linux-specific
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
    pkg_check_modules(FREETYPE REQUIRED freetype2)

    # Optional Linux libraries
    if(MIKO_ENABLE_SCREEN_CAPTURE)
        pkg_check_modules(X11 REQUIRED x11)
        pkg_check_modules(XRANDR REQUIRED xrandr)
    endif()

elseif(WIN32)
    # Windows-specific libraries
    if(MIKO_USE_VULKAN)
        # Vulkan handles most graphics needs
    else()
        # Additional Windows OpenGL support
        find_library(OPENGL32_LIB opengl32)
    endif()

    # Windows font and text rendering
    find_library(DWRITE_LIB dwrite)
    find_library(D2D1_LIB d2d1)
endif()

# Third-party dependencies
find_package(nlohmann_json REQUIRED)

# Image processing
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)

# Optional: HTTP client
if(MIKO_ENABLE_HTTP_CLIENT)
    find_package(CURL REQUIRED)
endif()