# Define source files
set(MIKO_CORE_SOURCES
    miko.cpp
    miko/app.cpp
    miko/window.cpp
    miko/view.cpp
    miko/element.cpp
    miko/context.cpp
    miko/style.cpp
    miko/layout.cpp
    miko/paint.cpp
    miko/events.cpp
    miko/animation.cpp
    miko/keymap.cpp
    miko/assets.cpp
    miko/text.cpp
    miko/image.cpp
    miko/svg.cpp
)

set(MIKO_PLATFORM_SDL2_SOURCES
    miko/platform/sdl2/window_sdl2.cpp
    miko/platform/sdl2/events_sdl2.cpp
    miko/platform/sdl2/input_sdl2.cpp
    miko/platform/sdl2/clipboard_sdl2.cpp
    miko/platform/sdl2/filesystem_sdl2.cpp
)

set(MIKO_PLATFORM_WINDOWS_SOURCES
    miko/platform/windows/text_windows.cpp
    miko/platform/windows/font_windows.cpp
    miko/platform/windows/system_windows.cpp
)

set(MIKO_PLATFORM_LINUX_SOURCES
    miko/platform/linux/text_linux.cpp
    miko/platform/linux/font_linux.cpp
    miko/platform/linux/system_linux.cpp
)

set(MIKO_RENDER_VULKAN_SOURCES
    miko/render/vulkan/renderer_vulkan.cpp
    miko/render/vulkan/device_vulkan.cpp
    miko/render/vulkan/swapchain_vulkan.cpp
    miko/render/vulkan/buffer_vulkan.cpp
    miko/render/vulkan/pipeline_vulkan.cpp
    miko/render/vulkan/shader_vulkan.cpp
)

set(MIKO_RENDER_OPENGL_SOURCES
    miko/render/opengl/renderer_opengl.cpp
    miko/render/opengl/context_opengl.cpp
    miko/render/opengl/buffer_opengl.cpp
    miko/render/opengl/shader_opengl.cpp
    miko/render/opengl/texture_opengl.cpp
)

set(MIKO_RENDER_COMMON_SOURCES
    miko/render/renderer.cpp
    miko/render/scene.cpp
    miko/render/quad.cpp
    miko/render/path.cpp
    miko/render/text_renderer.cpp
    miko/render/image_renderer.cpp
    miko/render/mesh.cpp
    miko/render/material.cpp
)

set(MIKO_TEXT_SOURCES
    miko/text/font_cache.cpp
    miko/text/text_layout.cpp
    miko/text/shaper.cpp
    miko/text/typography.cpp
)

set(MIKO_LAYOUT_SOURCES
    miko/layout/flexbox.cpp
    miko/layout/grid.cpp
    miko/layout/constraints.cpp
    miko/layout/tree.cpp
)

set(MIKO_UTIL_SOURCES
    miko/util/color.cpp
    miko/util/geometry.cpp
    miko/util/hash.cpp
    miko/util/uuid.cpp
    miko/util/thread_pool.cpp
    miko/util/timer.cpp
    miko/util/math.cpp
)

set(MIKO_ASSETS_SOURCES
    miko/assets/image_loader.cpp
    miko/assets/font_loader.cpp
    miko/assets/shader_loader.cpp
    miko/assets/resource_cache.cpp
)

set(MIKO_SCREEN_CAPTURE_SOURCES
    miko/capture/screen_capture.cpp
)

set(MIKO_SCREEN_CAPTURE_WINDOWS_SOURCES
    miko/capture/screen_capture_windows.cpp
)

set(MIKO_SCREEN_CAPTURE_LINUX_SOURCES
    miko/capture/screen_capture_linux.cpp
)

set(MIKO_HTTP_SOURCES
    miko/http/http_client.cpp
    miko/http/request.cpp
    miko/http/response.cpp
)

set(MIKO_INSPECTOR_SOURCES
    miko/inspector/inspector.cpp
    miko/inspector/debug_overlay.cpp
    miko/inspector/performance_monitor.cpp
)