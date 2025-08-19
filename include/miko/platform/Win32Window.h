#pragma once

#ifndef MIKO_WIN32WINDOW_H
#define MIKO_WIN32WINDOW_H

#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "../core/Window.h"
#include <windows.h>
#include <memory>

namespace miko {

    class D2DRenderer;

    class Win32Window : public Window {
    public:
        Win32Window();
        virtual ~Win32Window();
        
        // Window interface implementation
        bool Create(const std::string& title, int width, int height, WindowStyle style = WindowStyle::All) override;
        void Destroy() override;
        
        void SetTitle(const std::string& title) override;
        std::string GetTitle() const override;
        
        void SetSize(int width, int height) override;
        Size GetSize() const override;
        
        void SetPosition(int x, int y) override;
        Point GetPosition() const override;
        
        void SetVisible(bool visible) override;
        bool IsVisible() const override;
        
        void SetFocused(bool focused) override;
        bool IsFocused() const override;
        
        void SetMaximized(bool maximized) override;
        bool IsMaximized() const override;
        
        void SetMinimized(bool minimized) override;
        bool IsMinimized() const override;
        
        void Show() override;
        void Hide() override;
        void Close() override;
        void Invalidate() override;
        void Invalidate(const Rect& rect) override;
        
        void ProcessEvents() override;
        
        std::shared_ptr<Renderer> GetRenderer() const override;
        void Present() override;
        
        void SetMenuBar(void* menuBar) override;
        void* GetMenuBar() const override;
        
        void SetRootWidget(std::shared_ptr<Widget> widget) override;
        std::shared_ptr<Widget> GetRootWidget() const override;
        
        void* GetNativeHandle() const override { return hwnd; }
        
        // Win32 specific
        HWND GetHWND() const { return hwnd; }
        static Win32Window* FromHWND(HWND hwnd);
        
    private:
        HWND hwnd;
        HMENU menuBar;
        std::shared_ptr<D2DRenderer> renderer;
        bool isVisible;
        bool isDestroyed;
        
        // Window class registration
        static bool RegisterWindowClass();
        static const wchar_t* GetWindowClassName();
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
        
        // Message handlers
        void OnPaint();
        void OnSize(int width, int height);
        void OnClose();
        void OnDestroy();
        void OnMouseMove(int x, int y, WPARAM wParam);
        void OnMouseButton(UINT msg, int x, int y, WPARAM wParam);
        void OnMouseWheel(int x, int y, short delta, WPARAM wParam);
        void OnKeyDown(WPARAM wParam, LPARAM lParam);
        void OnKeyUp(WPARAM wParam, LPARAM lParam);
        void OnChar(WPARAM wParam, LPARAM lParam);
        void OnSetFocus();
        void OnKillFocus();
        
        // Helper methods
        DWORD GetWindowStyleFromMikoStyle(WindowStyle style);
        KeyCode VirtualKeyToMikoKey(WPARAM vk);
        bool IsKeyPressed(int vk);
        
        // DWM integration
        void EnableDWMComposition();
        void EnableDWMBlurBehind();
        void ExtendFrameIntoClientArea();
    };

} // namespace miko

#endif // _WIN32

#endif // MIKO_WIN32WINDOW_H