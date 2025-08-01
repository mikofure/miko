![](/assets/poster.png "A Modern Cross-Platform UI Framework with KWUi (Kawaii UI)")

# 🌸 Miko Framework

**A Modern Cross-Platform UI Framework with KWUi (Kawaii UI)**

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)](#supported-platforms)
[![C++](https://img.shields.io/badge/C%2B%2B-17%2B-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![SDL2](https://img.shields.io/badge/SDL2-2.0%2B-green.svg)](https://www.libsdl.org/)

## ✨ Overview

Miko Framework is a lightweight, modern C++ UI framework featuring **KWUi (Kawaii UI)** - a delightful and intuitive user interface system. Built on top of SDL2, Miko provides cross-platform native window management with beautiful, responsive UI components.

### 🎯 Key Features

- 🖼️ **Cross-Platform Native Windows** - Windows, Linux, and macOS support
- 🎨 **KWUi (Kawaii UI)** - Beautiful, modern UI components
- 🌙 **Automatic Theme Detection** - Dark/Light mode with system integration
- 📝 **Embedded Font System** - No external font dependencies
- 🎮 **SDL2 Powered** - Hardware-accelerated rendering
- 🏗️ **Modular Architecture** - Clean, extensible component system
- 🎪 **Context Menus & Dialogs** - Rich interactive elements
- 🎭 **Custom Titlebar** - Borderless windows with native controls

## 🚀 Quick Start

### Prerequisites

- **C++17** compatible compiler
- **CMake 3.16+**
- **SDL2** and **SDL2_ttf** (automatically fetched)

### Building

```bash
# Clone the repository
git clone https://github.com/mikofure/miko.git
cd miko

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build . --config Release

# Run the example
.\bin\Release\fck.exe
```

### Basic Usage

```cpp
#include "miko.hh"

int main() {
    // Initialize Miko Framework
    auto window = CreatePlatformWindow();
    
    // Setup KWUi components
    TitleBar titleBar;
    ContextItem contextMenu;
    
    // Configure theme
    ThemeManager::Instance().SetTheme(ThemeType::Auto);
    
    // Main loop
    window->Run();
    
    return 0;
}
```

## 🏗️ Architecture

### Core Components

```
Miko Framework
├── 🖼️ Platform Layer
│   ├── Windows (Win32 + SDL2)
│   ├── Linux (X11 + SDL2)
│   └── macOS (Cocoa + SDL2)
├── 🎨 KWUi (Kawaii UI)
│   ├── TitleBar
│   ├── MenuBar
│   ├── ContextItem
│   └── Theme System
├── 📦 Assets
│   ├── Embedded Fonts
│   ├── Icons
│   └── Local Resources
└── ⚙️ Configuration
    ├── App Config
    ├── Color Schemes
    └── Font Management
```

### Platform Abstraction

```cpp
// Cross-platform window creation
std::unique_ptr<PlatformWindow> CreatePlatformWindow();

// Platform-specific implementations
class WindowsNativeWindow : public PlatformWindow { /* ... */ };
class X11NativeWindow : public PlatformWindow { /* ... */ };
class CocoaNativeWindow : public PlatformWindow { /* ... */ };
```

## 🎨 KWUi (Kawaii UI) Components

### TitleBar

Custom borderless titlebar with native window controls:

```cpp
TitleBar titleBar;
titleBar.Initialize(hwnd, renderer);
titleBar.SetTitle(L"My Kawaii App");
titleBar.Render(windowWidth);
```

**Features:**
- Native minimize/maximize/close buttons
- Custom menu integration
- Drag-to-move functionality
- Theme-aware styling

### Context Menus

Rich, interactive context menus:

```cpp
ContextItem contextMenu;
contextMenu.AddItem("New File", []() { /* callback */ });
contextMenu.AddSeparator();
contextMenu.AddItem("Exit", []() { exit(0); });
contextMenu.Show(x, y);
```

**Features:**
- Hierarchical menus
- Icons and shortcuts
- Keyboard navigation
- Smooth animations

### Theme System

Automatic theme detection and switching:

```cpp
// Auto-detect system theme
ThemeManager::Instance().SetTheme(ThemeType::Auto);

// Manual theme selection
ThemeManager::Instance().SetTheme(ThemeType::Dark);

// Register for theme changes
ThemeManager::Instance().RegisterThemeChangeCallback(
    [](const Theme& newTheme) {
        // Update UI components
    }
);
```

**Supported Themes:**
- 🌞 **Light Theme** - Clean, bright interface
- 🌙 **Dark Theme** - Modern dark interface
- ♿ **High Contrast** - Accessibility-focused
- 🔄 **Auto Theme** - Follows system preference

## 📦 Asset Management

### Embedded Fonts

Fonts are compiled directly into the executable:

```cpp
// Load fonts from binary data
auto font = LocalResources::Instance().LoadFont(
    LocalResources::FontType::MonaSans_Regular, 14
);
```

**Included Fonts:**
- **MonaSans** (Regular, Bold, Medium, Light)
- **Segoe Fluent Icons** (UI icons)

### Resource Loading

```cpp
// Centralized resource management
LocalResources& resources = LocalResources::Instance();
TTF_Font* font = resources.LoadFont(FontType::MonaSans_Bold, 16);
```

## 🌍 Supported Platforms

| Platform | Status | Features |
|----------|--------|---------|
| **Windows 10/11** | ✅ Full Support | Native theming, DWM integration |
| **Linux (X11)** | ✅ Full Support | GTK theme detection |
| **macOS** | 🚧 In Progress | Cocoa integration planned |

## 🛠️ Development

### Project Structure

```
miko/
├── assets/           # Embedded resources
│   ├── fonts/       # Binary font data
│   └── localresources.{hh,cc}
├── components/       # UI components
│   └── contextitem.{hh,cc}
├── config/          # Configuration system
│   └── appconfig.hh
├── gui/             # Core UI elements
│   ├── titlebar.{hh,cc}
│   └── menubar.{hh,cc}
├── platform/        # Platform abstraction
│   ├── platform_window.{hh,cc}
│   ├── windows/     # Windows implementation
│   └── linux/       # Linux implementation
└── variables/       # Theme and styling
    ├── color.hh
    ├── font.hh
    └── theme.{hh,cc}
```

### Building as Library

```cmake
# In your CMakeLists.txt
find_package(miko REQUIRED)
target_link_libraries(your_app miko::framework)
```

### Custom Components

```cpp
class MyCustomComponent {
public:
    void Initialize(SDL_Renderer* renderer) {
        // Use current theme
        auto& theme = GetCurrentTheme();
        m_backgroundColor = theme.colors.windowBackground;
    }
    
    void Render() {
        // Render with theme colors
        SDL_SetRenderDrawColor(m_renderer, 
            m_backgroundColor.r, m_backgroundColor.g, 
            m_backgroundColor.b, m_backgroundColor.a);
    }
};
```

## 📚 API Reference

### Core Classes

- **`PlatformWindow`** - Cross-platform window abstraction
- **`TitleBar`** - Custom titlebar component
- **`ContextItem`** - Context menu system
- **`ThemeManager`** - Theme management and detection
- **`LocalResources`** - Embedded asset management
- **`AppConfig`** - Application configuration

### Theme System

- **`ThemeType`** - Theme enumeration (Auto, Light, Dark, HighContrast)
- **`ColorScheme`** - Color definitions for themes
- **`FontScheme`** - Font configurations
- **`Colors`** - Color utility functions
- **`Fonts`** - Font management utilities

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guide](CONTRIBUTING.md) for details.

### Development Setup

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

### Code Style

- Follow C++17 standards
- Use descriptive variable names
- Comment complex algorithms
- Maintain cross-platform compatibility

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **SDL2** - Cross-platform multimedia library
- **MonaSans** - Beautiful font family
- **Segoe Fluent Icons** - Microsoft's icon font
- The open-source community for inspiration and support

## 📞 Support

- 🐛 **Issues**: [GitHub Issues](https://github.com/mikofure/miko/issues)
- 📖 **Documentation**: [docs.miko-framework.dev](https://docs.mikofure.io)

---

<div align="center">

**Made with 💖 by the Miko Framework Team**

*Building beautiful, cross-platform applications has never been easier!*

</div>
