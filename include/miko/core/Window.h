#pragma once

#ifndef MIKO_WINDOW_H
#define MIKO_WINDOW_H

#include "../utils/Math.h"
#include "../utils/Event.h"
#include <string>
#include <memory>
#include <functional>

namespace miko {

    class Renderer;
    class Widget;

    enum class WindowStyle {
        Default = 0,
        Borderless = 1,
        Resizable = 2,
        Maximizable = 4,
        Minimizable = 8,
        Closable = 16,
        All = Resizable | Maximizable | Minimizable | Closable
    };

    class Window {
    public:
        Window() = default;
        virtual ~Window() = default;
        
        // Window creation and destruction
        virtual bool Create(const std::string& title, int width, int height, WindowStyle style = WindowStyle::All) = 0;
        virtual void Destroy() = 0;
        
        // Window properties
        virtual void SetTitle(const std::string& title) = 0;
        virtual std::string GetTitle() const = 0;
        
        virtual void SetSize(int width, int height) = 0;
        virtual Size GetSize() const = 0;
        
        virtual void SetPosition(int x, int y) = 0;
        virtual Point GetPosition() const = 0;
        
        virtual void SetVisible(bool visible) = 0;
        virtual bool IsVisible() const = 0;
        
        virtual void SetFocused(bool focused) = 0;
        virtual bool IsFocused() const = 0;
        
        virtual void SetMaximized(bool maximized) = 0;
        virtual bool IsMaximized() const = 0;
        
        virtual void SetMinimized(bool minimized) = 0;
        virtual bool IsMinimized() const = 0;
        
        // Window operations
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual void Close() = 0;
        virtual void Invalidate() = 0;
        virtual void Invalidate(const Rect& rect) = 0;
        
        // Event handling
        virtual void ProcessEvents() = 0;
        
        // Rendering
        virtual std::shared_ptr<Renderer> GetRenderer() const = 0;
        virtual void Present() = 0;
        
        // Widget management
        virtual void SetRootWidget(std::shared_ptr<Widget> widget) { rootWidget = widget; }
        virtual std::shared_ptr<Widget> GetRootWidget() const { return rootWidget; }
        
        // Menu bar
        virtual void SetMenuBar(void* menuBar) = 0;
        virtual void* GetMenuBar() const = 0;
        
        // Platform specific handle
        virtual void* GetNativeHandle() const = 0;
        
        // Event callbacks
        std::function<void(const WindowEvent&)> OnResize;
        std::function<void(const WindowEvent&)> OnClose;
        std::function<void(const WindowEvent&)> OnFocus;
        std::function<void(const MouseEvent&)> OnMouseEvent;
        std::function<void(const KeyEvent&)> OnKeyEvent;
        
    protected:
        std::shared_ptr<Widget> rootWidget;
        
        // Helper methods for derived classes
        virtual void DispatchEvent(const Event& event);
        virtual void UpdateLayout();
        virtual void RenderWidgets();
    };

    // Factory function for creating platform-specific windows
    std::shared_ptr<Window> CreatePlatformWindow();

} // namespace miko

#endif // MIKO_WINDOW_H