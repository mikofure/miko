![](/assets/poster.png "A Modern Cross-Platform UI Framework with KWUi (Kawaii UI)")

# 🌸 Miko Framework

**A Modern Cross-Platform C++ GUI Framework using SDL2 and Vulkan/OpenGL**

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey.svg)](#supported-platforms)
[![C++](https://img.shields.io/badge/C%2B%2B-20%2B-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![SDL2](https://img.shields.io/badge/SDL2-2.0%2B-green.svg)](https://www.libsdl.org/)
[![Vulkan](https://img.shields.io/badge/Vulkan-1.3%2B-red.svg)](https://www.vulkan.org/)

## ✨ Overview

Miko Framework is a modern, GPU-accelerated C++ UI framework designed for high-performance cross-platform applications. Built on SDL2 with Vulkan and OpenGL rendering backends, Miko provides a comprehensive solution for creating beautiful, responsive user interfaces.

### 🎯 Key Features

- 🖼️ **Cross-Platform Support** - Windows and Linux (macOS not supported)
- 🎨 **GPU-Accelerated Rendering** - Vulkan and OpenGL backends
- 🏗️ **Modular CMake Architecture** - Clean, maintainable build system
- 📦 **Auto-Dependency Management** - Automatic GitHub source fetching
- 🎮 **SDL2 Integration** - Hardware-accelerated windowing and input
- 🎪 **Rich UI Components** - Layout engines, text rendering, animations
- 🔧 **Developer Tools** - Inspector, performance monitoring, debugging
- 📸 **Screen Capture** - Built-in screen recording capabilities
- 🌐 **HTTP Client** - Integrated networking support

## 🚀 Quick Start

### Prerequisites

- **C++20** compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- **CMake 3.25+**
- **Vulkan SDK** (for Vulkan backend) or **OpenGL 4.5+**
- **SDL2** (automatically fetched if not found)

### Building

```bash
# Clone the repository
git clone https://github.com/mikofure/miko.git
cd miko

# Create build directory
mkdir build && cd build

# Configure with auto-dependency fetching
cmake -DMIKO_AUTO_BUILD_DEPS=ON ..

# Build the framework
cmake --build . --config Release

# Run examples
.\examples\hello_world.exe
```

### Basic Usage

```cpp
#include "miko.h"

int main() {
    // Initialize Miko Framework
    miko::App app;
    
    // Create main window
    auto window = app.CreateWindow("My Miko App", 800, 600);
    
    // Setup UI elements
    auto view = std::make_shared<miko::View>();
    window->SetRootView(view);
    
    // Run the application
    return app.Run();
}
```

## 🏗️ Architecture

### Project Structure

```
miko/
├── CMakeLists.txt           # Main CMake configuration
├── CMakes/                  # Modular CMake files
│   ├── Options.cmake        # Build options and features
│   ├── Platform.cmake       # Platform detection and setup
│   ├── ThirdParty.cmake     # Auto-dependency management
│   ├── Dependencies.cmake   # Package finding and linking
│   ├── Sources.cmake        # Source file organization
│   ├── Target.cmake         # Library target configuration
│   ├── Examples.cmake       # Example applications
│   ├── Installation.cmake   # Install and packaging
│   └── Utils.cmake          # Utilities and summary
├── miko.cpp                 # Framework entry point
├── miko.h                   # Main header file
└── miko/                    # Core framework modules
    ├── app.{cpp,h}          # Application management
    ├── window.{cpp,h}       # Window abstraction
    ├── view.{cpp,h}         # View hierarchy
    ├── element.{cpp,h}      # UI elements
    ├── context.{cpp,h}      # Rendering context
    ├── style.{cpp,h}        # Styling system
    ├── layout.{cpp,h}       # Layout management
    ├── paint.{cpp,h}        # Painting operations
    ├── events.{cpp,h}       # Event handling
    ├── animation.{cpp,h}    # Animation system
    ├── assets.{cpp,h}       # Asset management
    ├── text.{cpp,h}         # Text rendering
    ├── image.{cpp,h}        # Image handling
    ├── svg.{cpp,h}          # SVG support
    ├── platform/            # Platform abstraction
    │   ├── sdl2/           # SDL2 implementation
    │   ├── windows/        # Windows-specific code
    │   └── linux/          # Linux-specific code
    ├── render/             # Rendering backends
    │   ├── vulkan/         # Vulkan renderer
    │   ├── opengl/         # OpenGL renderer
    │   └── *.{cpp,h}       # Common rendering
    ├── text/               # Text processing
    │   └── *.{cpp,h}       # Font cache, layout, shaping
    ├── layout/             # Layout engines
    │   └── *.{cpp,h}       # Flexbox, grid, constraints
    ├── util/               # Utilities
    │   └── *.{cpp,h}       # Math, color, geometry, etc.
    ├── assets/             # Asset loaders
    │   └── *.{cpp,h}       # Image, font, shader loaders
    ├── capture/            # Screen capture
    │   └── *.{cpp,h}       # Platform-specific capture
    ├── http/               # HTTP client
    │   └── *.{cpp,h}       # Request, response, client
    └── inspector/          # Developer tools
        └── *.{cpp,h}       # Debug overlay, profiling
```

### Core Components

#### Rendering System
- **Vulkan Backend** - High-performance, low-overhead rendering
- **OpenGL Backend** - Broad compatibility fallback
- **Scene Graph** - Hierarchical rendering organization
- **Material System** - Shader and texture management

#### Layout Engine
- **Flexbox Layout** - CSS-like flexible layouts
- **Grid Layout** - Two-dimensional grid systems
- **Constraint Layout** - Relationship-based positioning
- **Layout Tree** - Efficient layout calculation

#### Platform Abstraction
- **SDL2 Integration** - Cross-platform windowing
- **Platform-Specific Code** - Native optimizations
- **Input Handling** - Unified input abstraction
- **File System** - Cross-platform file operations

## 🎨 Features

### Rendering Backends

```cpp
// Choose rendering backend
cmake -DMIKO_USE_VULKAN=ON ..     # Use Vulkan (default)
cmake -DMIKO_USE_VULKAN=OFF ..    # Use OpenGL
```

### Optional Features

```cpp
// Enable optional components
cmake -DMIKO_ENABLE_SCREEN_CAPTURE=ON ..
cmake -DMIKO_ENABLE_HTTP_CLIENT=ON ..
cmake -DMIKO_ENABLE_INSPECTOR=ON ..
cmake -DMIKO_ENABLE_LEAK_DETECTION=ON ..
```

### Auto-Dependency Management

```cpp
// Automatically fetch missing dependencies from GitHub
cmake -DMIKO_AUTO_BUILD_DEPS=ON ..
```

**Auto-fetched Dependencies:**
- SDL2 from libsdl-org/SDL
- nlohmann_json from nlohmann/json
- GLEW from nigels-com/glew (OpenGL only)
- Google Test from google/googletest (tests only)

### Text and Typography

```cpp
// Rich text rendering
auto textRenderer = miko::TextRenderer();
textRenderer.SetFont("Arial", 14);
textRenderer.SetColor(miko::Color::Black);
textRenderer.RenderText("Hello, Miko!", x, y);
```

### Animation System

```cpp
// Smooth animations
auto animation = miko::Animation::Create()
    .From(0.0f).To(1.0f)
    .Duration(1000ms)
    .Easing(miko::Easing::EaseInOut)
    .OnUpdate([](float value) {
        // Update UI element
    });
```

## 🌍 Supported Platforms

| Platform | Status | Graphics APIs | Notes |
|----------|--------|---------------|-------|
| **Windows 10/11** | ✅ Full Support | Vulkan, OpenGL | DirectWrite, D2D1 integration |
| **Linux (X11)** | ✅ Full Support | Vulkan, OpenGL | FontConfig, FreeType support |
| **macOS** | ❌ Not Supported | - | Explicitly disabled |

## 🛠️ Development

### Build Options

```cmake
# Core options
option(MIKO_BUILD_EXAMPLES "Build Miko examples" ON)
option(MIKO_BUILD_TESTS "Build Miko tests" ON)
option(MIKO_USE_VULKAN "Use Vulkan renderer" ON)

# Optional features
option(MIKO_ENABLE_SCREEN_CAPTURE "Enable screen capture" OFF)
option(MIKO_ENABLE_INSPECTOR "Enable UI inspector" OFF)
option(MIKO_ENABLE_HTTP_CLIENT "Enable HTTP client" ON)
option(MIKO_ENABLE_LEAK_DETECTION "Enable memory leak detection" OFF)
option(MIKO_RUNTIME_SHADERS "Load shaders at runtime" OFF)

# Auto-dependency management
option(MIKO_AUTO_BUILD_DEPS "Auto-build missing dependencies" ON)
```

### Examples

The framework includes comprehensive examples:

- **hello_world** - Basic window and rendering
- **image_viewer** - Image loading and display
- **input_demo** - Input handling showcase
- **text_editor** - Rich text editing
- **window_styling** - Custom window appearance
- **svg_viewer** - SVG rendering support
- **menu_demo** - Context menus and navigation
- **animation_demo** - Animation system showcase

### Testing

```bash
# Build and run tests
cmake -DMIKO_BUILD_TESTS=ON ..
cmake --build . --target test
```

### Memory Leak Detection

```bash
# Enable AddressSanitizer (Linux/GCC)
cmake -DMIKO_ENABLE_LEAK_DETECTION=ON ..

# Windows uses CRT debug heap automatically
```

## 📚 API Reference

### Core Classes

- **`miko::App`** - Application lifecycle management
- **`miko::Window`** - Window creation and management
- **`miko::View`** - UI view hierarchy
- **`miko::Element`** - Base UI element class
- **`miko::Renderer`** - Graphics rendering abstraction
- **`miko::Layout`** - Layout calculation engine
- **`miko::Style`** - Styling and theming system

### Rendering

- **`miko::VulkanRenderer`** - Vulkan-based renderer
- **`miko::OpenGLRenderer`** - OpenGL-based renderer
- **`miko::Scene`** - Scene graph management
- **`miko::Material`** - Shader and texture handling

### Utilities

- **`miko::Color`** - Color manipulation
- **`miko::Geometry`** - Geometric calculations
- **`miko::ThreadPool`** - Asynchronous task execution
- **`miko::Timer`** - High-precision timing

## 🤝 Contributing

We welcome contributions! Please follow these guidelines:

1. **Fork** the repository
2. **Create** a feature branch
3. **Follow** C++20 coding standards
4. **Add** tests for new functionality
5. **Ensure** cross-platform compatibility
6. **Submit** a pull request

### Code Style

- Use C++20 features appropriately
- Follow RAII principles
- Prefer `std::unique_ptr` and `std::shared_ptr`
- Use meaningful variable and function names
- Comment complex algorithms and design decisions

## 📄 License

This project is licensed under the MIT License:

```
Copyright (c) 2025 Aika (Ariz Kamizuki)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHOR OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## 🙏 Acknowledgments

- **SDL2** - Cross-platform multimedia library
- **Vulkan** - High-performance graphics API
- **nlohmann/json** - Modern JSON library for C++
- **Google Test** - C++ testing framework
- The open-source community for inspiration and support

## 📞 Support

- 🐛 **Issues**: [GitHub Issues](https://github.com/mikofure/miko/issues)
- 📖 **Documentation**: [Wiki](https://github.com/mikofure/miko/wiki)
- 💬 **Discussions**: [GitHub Discussions](https://github.com/mikofure/miko/discussions)

---

<div align="center">

**Made with 💖 by the Miko Framework Team**

*Building high-performance, cross-platform applications with modern C++*

</div>