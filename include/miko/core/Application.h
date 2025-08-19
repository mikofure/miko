#pragma once

#ifndef MIKO_APPLICATION_H
#define MIKO_APPLICATION_H

#include "../utils/Event.h"
#include <memory>
#include <vector>
#include <string>

namespace miko {

    class Window;
    class Renderer;

    class Application {
    public:
        Application();
        virtual ~Application();
        
        // Singleton access
        static Application* GetInstance();
        
        // Application lifecycle
        virtual bool Initialize();
        virtual void Run();
        virtual void Shutdown();
        
        // Window management
        std::shared_ptr<Window> CreateWindow(const std::string& title, int width, int height);
        void CloseWindow(std::shared_ptr<Window> window);
        std::shared_ptr<Window> GetMainWindow() const { return mainWindow; }
        
        // Event handling
        void ProcessEvents();
        virtual void OnEvent(const Event& event) {}
        
        // Application properties
        const std::string& GetName() const { return appName; }
        void SetName(const std::string& name) { appName = name; }
        
        bool IsRunning() const { return running; }
        void Quit() { running = false; }
        
        // Frame timing
        float GetDeltaTime() const { return deltaTime; }
        float GetFPS() const { return fps; }
        
    protected:
        virtual void Update(float deltaTime) {}
        virtual void Render() {}
        
    private:
        static Application* instance;
        
        std::string appName;
        bool running;
        bool initialized;
        
        std::shared_ptr<Window> mainWindow;
        std::vector<std::shared_ptr<Window>> windows;
        
        // Timing
        float deltaTime;
        float fps;
        uint64_t lastFrameTime;
        
        void UpdateTiming();
        void OnWindowClose(const WindowEvent& event);
    };

    // Macro to define main application entry point
    #define MIKO_IMPLEMENT_APPLICATION(className) \
        int main() { \
            auto app = std::make_unique<className>(); \
            if (!app->Initialize()) { \
                return -1; \
            } \
            app->Run(); \
            app->Shutdown(); \
            return 0; \
        }

} // namespace miko

#endif // MIKO_APPLICATION_H