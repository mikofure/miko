#pragma once

#ifndef MIKO_H
#define MIKO_H

/**
 * @brief Miko GUI Framework
 * 
 * A modern, lightweight GUI framework for Windows applications.
 * Built with Direct2D for hardware-accelerated rendering and
 * Win32 for native window management.
 * 
 * Key Features:
 * - Hardware-accelerated rendering with Direct2D
 * - Modern C++17 design with smart pointers
 * - Flexible layout system (Stack and Grid layouts)
 * - Event-driven architecture
 * - DWM integration for modern window effects
 * - Comprehensive widget library
 * 
 * Basic Usage:
 * @code
 * #include <miko/miko.h>
 * 
 * class MyApp : public miko::Application {
 * public:
 *     bool Initialize() override {
 *         if (!Application::Initialize()) return false;
 *         
 *         auto window = CreateWindow("My App", 800, 600);
 *         // Set up UI...
 *         window->Show();
 *         
 *         return true;
 *     }
 * };
 * 
 * MIKO_IMPLEMENT_APPLICATION(MyApp)
 * @endcode
 */

// Core framework headers
#include "core/Application.h"
#include "core/Window.h"
#include "core/Renderer.h"

// Widget system headers
#include "widgets/Widget.h"
#include "widgets/Button.h"
#include "widgets/Label.h"
#include "widgets/TextBox.h"
#include "widgets/Panel.h"

// Layout system headers
#include "layout/Layout.h"
#include "layout/StackLayout.h"
#include "layout/GridLayout.h"

// Utility headers
#include "utils/Math.h"
#include "utils/Color.h"
#include "utils/Event.h"

// Platform specific headers
#ifdef _WIN32
#include "platform/Win32Window.h"
#include "platform/D2DRenderer.h"
#endif

namespace miko {
    // Framework version
    constexpr int VERSION_MAJOR = 1;
    constexpr int VERSION_MINOR = 0;
    constexpr int VERSION_PATCH = 0;
    
    // Framework initialization and utility functions
    bool Initialize();
    void Shutdown();
    bool IsInitialized();
    std::string GetVersion();
}

#endif // MIKO_H