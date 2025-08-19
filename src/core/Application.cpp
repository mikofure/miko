#include "miko/core/Application.h"
#include "miko/platform/Win32Window.h"
#include "miko/miko.h"
#include <windows.h>
#include <objbase.h>
#include <chrono>
#include <algorithm>
#include <functional>

// Prevent Windows API macros from interfering with our method names
#ifdef CreateWindow
#undef CreateWindow
#endif

namespace miko {

Application* Application::instance = nullptr;

Application::Application()
    : appName("Miko Application")
    , running(false)
    , initialized(false)
    , deltaTime(0.0f)
    , fps(0.0f)
    , lastFrameTime(0)
{
    instance = this;
}

Application::~Application() {
    instance = nullptr;
}

bool Application::Initialize() {
    if (initialized) {
        return true;
    }
    
    // Initialize the Miko framework
    if (!miko::Initialize()) {
        return false;
    }
    
    running = true;
    initialized = true;
    
    // Initialize timing
    auto now = std::chrono::high_resolution_clock::now();
    lastFrameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    
    return true;
}

void Application::Run() {
    if (!running) {
        return;
    }
    
    MSG msg = {};
    
    while (running) {
        // Update timing
        UpdateTiming();
        
        // Process Windows messages
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
                break;
            }
            
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        if (!running) break;
        
        // Update application
        Update(deltaTime);
        
        // Render all windows
        for (auto& window : windows) {
            if (window && window->IsVisible()) {
                window->Present();
            }
        }
        
        // Small sleep to prevent 100% CPU usage
        Sleep(1);
    }
}

void Application::Shutdown() {
    running = false;
    
    // Close all windows
    for (auto& window : windows) {
        if (window) {
            window->Close();
        }
    }
    windows.clear();
    mainWindow.reset();
    
    // Shutdown the Miko framework
    miko::Shutdown();
}

std::shared_ptr<Window> Application::CreateWindow(const std::string& title, int width, int height) {
    auto window = std::make_shared<Win32Window>();
    
    if (!window->Create(title, width, height)) {
        return nullptr;
    }
    
    // TODO: Set up window close callback
    // window->OnClose = [this, window](const WindowEvent& event) {
    //     CloseWindow(window);
    // };
    
    windows.push_back(window);
    
    // Set as main window if it's the first one
    if (!mainWindow) {
        mainWindow = window;
    }
    
    return window;
}

void Application::CloseWindow(std::shared_ptr<Window> window) {
    if (!window) return;
    
    // Remove from windows list
    auto it = std::find(windows.begin(), windows.end(), window);
    if (it != windows.end()) {
        windows.erase(it);
    }
    
    // If this was the main window, exit the application
    if (window == mainWindow) {
        running = false;
    }
    
    // Destroy the window
    window->Destroy();
}

Application* Application::GetInstance() {
    return instance;
}

void Application::UpdateTiming() {
    auto now = std::chrono::high_resolution_clock::now();
    uint64_t currentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
    deltaTime = (currentTime - lastFrameTime) / 1e9f;
    lastFrameTime = currentTime;
}

} // namespace miko