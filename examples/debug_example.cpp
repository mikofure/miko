#include <miko/miko.h>
#include <iostream>

// Prevent Windows API macros from interfering with our method names
#ifdef CreateWindow
#undef CreateWindow
#endif

using namespace miko;

class DebugExampleApp : public Application {
public:
    bool Initialize() override {
        std::cout << "DebugExampleApp::Initialize() called" << std::endl;
        
        if (!Application::Initialize()) {
            std::cout << "Application::Initialize() failed" << std::endl;
            return false;
        }
        std::cout << "Application::Initialize() succeeded" << std::endl;
        
        // Create main window
        auto window = CreateWindow("Debug Example", 400, 300);
        if (!window) {
            std::cout << "CreateWindow failed" << std::endl;
            return false;
        }
        std::cout << "CreateWindow succeeded" << std::endl;
        
        // Show the window
        window->Show();
        std::cout << "Window shown" << std::endl;
        
        return true;
    }
    
    void Update(float deltaTime) override {
        // Empty for now
    }
    
    void Render() override {
        // Empty for now
    }
};

MIKO_IMPLEMENT_APPLICATION(DebugExampleApp)