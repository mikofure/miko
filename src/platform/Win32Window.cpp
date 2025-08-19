#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "miko/platform/Win32Window.h"
#include "miko/platform/D2DRenderer.h"
#include "miko/utils/Event.h"
#include "miko/widgets/Widget.h"
#include <dwmapi.h>
#include <windowsx.h>
#include <iostream>

// Undefine Windows macros that conflict with our method names
#ifdef IsMaximized
#undef IsMaximized
#endif
#ifdef IsMinimized
#undef IsMinimized
#endif

#pragma comment(lib, "dwmapi.lib")

namespace miko {

static const wchar_t* WINDOW_CLASS_NAME = L"MikoWindow";
static bool s_windowClassRegistered = false;

Win32Window::Win32Window()
: hwnd(nullptr)
, renderer(nullptr)
, isVisible(false)
, isDestroyed(false)
{
}

Win32Window::~Win32Window() {
    Destroy();
}

bool Win32Window::Create(const std::string& title, int width, int height, WindowStyle style) {
    if (!RegisterWindowClass()) {
        std::cout << "Win32Window::Create - RegisterWindowClass failed" << std::endl;
        return false;
    }
    std::cout << "Win32Window::Create - RegisterWindowClass succeeded" << std::endl;
    
    // Convert title to wide string
    int titleLen = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, nullptr, 0);
    std::wstring wideTitle(titleLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, &wideTitle[0], titleLen);
    
    // Calculate window size including borders
    RECT rect = { 0, 0, width, height };
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, FALSE, 0);
    
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;
    
    // Center window on screen
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;
    
    // Create window
    std::cout << "Creating window with params: x=" << x << ", y=" << y << ", width=" << windowWidth << ", height=" << windowHeight << std::endl;
    hwnd = CreateWindowExW(
        0,
        WINDOW_CLASS_NAME,
        wideTitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        x, y, windowWidth, windowHeight,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        this
    );
    std::cout << "CreateWindowExW returned hwnd: " << hwnd << std::endl;
    
    if (!hwnd) {
        DWORD error = GetLastError();
        std::cout << "Win32Window::Create - CreateWindowExW failed with error: " << error << std::endl;
        return false;
    }
    std::cout << "Win32Window::Create - CreateWindowExW succeeded" << std::endl;
    
    // Enable DWM composition for modern effects
    EnableDWMComposition();
    
    // Create renderer
    renderer = std::make_shared<D2DRenderer>();
    if (!renderer->Initialize(hwnd)) {
        std::cout << "Win32Window::Create - D2DRenderer::Initialize failed" << std::endl;
        return false;
    }
    std::cout << "Win32Window::Create - D2DRenderer::Initialize succeeded" << std::endl;
    
    return true;
}

void Win32Window::Destroy() {
    if (renderer) {
        renderer->Shutdown();
        renderer.reset();
    }
    
    if (hwnd) {
        DestroyWindow(hwnd);
        hwnd = nullptr;
    }
}

void Win32Window::Show() {
    if (hwnd) {
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
        isVisible = true;
    }
}

void Win32Window::Hide() {
    if (hwnd) {
        ShowWindow(hwnd, SW_HIDE);
        isVisible = false;
    }
}

void Win32Window::Close() {
    if (hwnd) {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
    }
}

void Win32Window::SetTitle(const std::string& title) {
    if (hwnd) {
        int titleLen = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, nullptr, 0);
        std::wstring wideTitle(titleLen, 0);
        MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, &wideTitle[0], titleLen);
        SetWindowTextW(hwnd, wideTitle.c_str());
    }
}

std::string Win32Window::GetTitle() const {
    if (!hwnd) return "";
    
    int length = GetWindowTextLengthW(hwnd);
    if (length == 0) return "";
    
    std::wstring wideTitle(length + 1, 0);
    GetWindowTextW(hwnd, &wideTitle[0], length + 1);
    
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideTitle.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string title(utf8Len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideTitle.c_str(), -1, &title[0], utf8Len, nullptr, nullptr);
    
    return title;
}

void Win32Window::SetSize(int width, int height) {
    if (hwnd) {
        RECT rect = { 0, 0, width, height };
        AdjustWindowRectEx(&rect, GetWindowLong(hwnd, GWL_STYLE), menuBar != nullptr, GetWindowLong(hwnd, GWL_EXSTYLE));
        
        SetWindowPos(hwnd, nullptr, 0, 0, 
                    rect.right - rect.left, rect.bottom - rect.top,
                    SWP_NOMOVE | SWP_NOZORDER);
    }
}

Size Win32Window::GetSize() const {
    if (!hwnd) return Size(0, 0);
    
    RECT rect;
    GetClientRect(hwnd, &rect);
    return Size((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
}

void Win32Window::SetPosition(int x, int y) {
    if (hwnd) {
        SetWindowPos(hwnd, nullptr, x, y, 0, 0,
                    SWP_NOSIZE | SWP_NOZORDER);
    }
}

Point Win32Window::GetPosition() const {
    if (!hwnd) return Point(0, 0);
    
    RECT rect;
    GetWindowRect(hwnd, &rect);
    return Point((float)rect.left, (float)rect.top);
}

void Win32Window::SetVisible(bool visible) {
    if (visible) {
        Show();
    } else {
        Hide();
    }
}

bool Win32Window::IsVisible() const {
    return isVisible;
}

void Win32Window::SetFocused(bool focused) {
    if (hwnd && focused) {
        SetFocus(hwnd);
        SetForegroundWindow(hwnd);
    }
}

bool Win32Window::IsFocused() const {
    return hwnd && GetFocus() == hwnd;
}

void Win32Window::SetMaximized(bool maximized) {
    if (hwnd) {
        ShowWindow(hwnd, maximized ? SW_MAXIMIZE : SW_RESTORE);
    }
}

void Win32Window::SetMinimized(bool minimized) {
    if (hwnd) {
        ShowWindow(hwnd, minimized ? SW_MINIMIZE : SW_RESTORE);
    }
}



bool Win32Window::IsMaximized() const {
    if (!this->hwnd) return false;
    return ::IsZoomed(this->hwnd) != 0;
}

bool Win32Window::IsMinimized() const {
    if (!this->hwnd) return false;
    return ::IsIconic(this->hwnd) != 0;
}

void Win32Window::Invalidate() {
    if (this->hwnd) {
        InvalidateRect(this->hwnd, nullptr, FALSE);
    }
}

void Win32Window::Invalidate(const Rect& rect) {
    if (this->hwnd) {
        RECT winRect = {
            static_cast<LONG>(rect.x),
            static_cast<LONG>(rect.y),
            static_cast<LONG>(rect.x + rect.width),
            static_cast<LONG>(rect.y + rect.height)
        };
        InvalidateRect(this->hwnd, &winRect, FALSE);
    }
}

void Win32Window::ProcessEvents() {
    MSG msg;
    while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

std::shared_ptr<Renderer> Win32Window::GetRenderer() const {
    return renderer;
}

void Win32Window::Present() {
    if (renderer && rootWidget) {
        renderer->BeginDraw();
        
        // Clear background
        renderer->Clear(Color(240, 240, 240, 255));
        
        // Render root widget
        rootWidget->Render(std::static_pointer_cast<Renderer>(renderer));
        
        renderer->EndDraw();
    }
}

void Win32Window::SetRootWidget(std::shared_ptr<Widget> widget) {
    rootWidget = widget;
    if (widget && hwnd) {
        // Set widget size to match window client area
        Size windowSize = GetSize();
        widget->SetSize(windowSize);
        widget->Arrange(Rect(0, 0, windowSize.width, windowSize.height));
    }
}

std::shared_ptr<Widget> Win32Window::GetRootWidget() const {
    return rootWidget;
}

void Win32Window::SetMenuBar(void* menuBar) {
    this->menuBar = static_cast<HMENU>(menuBar);
    if (hwnd) {
        SetMenu(hwnd, this->menuBar);
    }
}

void* Win32Window::GetMenuBar() const {
    return menuBar;
}

bool Win32Window::RegisterWindowClass() {
    if (s_windowClassRegistered) {
        return true;
    }
    
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr; // We'll handle painting ourselves
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    
    if (!RegisterClassExW(&wc)) {
        return false;
    }
    
    s_windowClassRegistered = true;
    return true;
}

void Win32Window::EnableDWMComposition() {
    if (this->hwnd) {
        // Enable DWM composition
        BOOL enabled = TRUE;
        DwmEnableComposition(enabled);
        
        // Extend frame into client area for modern look
        MARGINS margins = { -1, -1, -1, -1 };
        DwmExtendFrameIntoClientArea(this->hwnd, &margins);
    }
}

LRESULT CALLBACK Win32Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Win32Window* window = nullptr;
    
    if (msg == WM_NCCREATE) {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = static_cast<Win32Window*>(cs->lpCreateParams);
        if (window) {
            window->hwnd = hwnd;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        }
    } else {
        window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    
    if (window) {
        return window->HandleMessage(msg, wParam, lParam);
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Win32Window::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // Window is being created, we can perform initialization here
            return 0;
        }
        
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            
            if (renderer) {
                renderer->Resize(width, height);
            }
            
            if (rootWidget) {
                rootWidget->SetSize(Size((float)width, (float)height));
                rootWidget->Arrange(Rect(0, 0, (float)width, (float)height));
            }
            
            if (OnResize) {
                WindowEvent event;
                event.type = EventType::WindowResize;
                event.width = width;
                event.height = height;
                OnResize(event);
            }
            
            // Update maximized/minimized state handled by IsMaximized/IsMinimized methods
            
            return 0;
        }
        
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            if (renderer) {
                renderer->BeginDraw();
                if (rootWidget) {
                    rootWidget->Render(renderer);
                }
                renderer->EndDraw();
            }
            EndPaint(hwnd, &ps);
            return 0;
        }
        
        case WM_CLOSE: {
            // Immediately destroy the window for faster close response
            DestroyWindow(hwnd);
            return 0;
        }
        
        case WM_SETFOCUS: {
            if (OnFocus) {
                WindowEvent event;
                event.type = EventType::WindowFocus;
                OnFocus(event);
            }
            return 0;
        }
        
        case WM_KILLFOCUS: {
            if (OnFocus) {
                WindowEvent event;
                event.type = EventType::WindowLostFocus;
                OnFocus(event);
            }
            return 0;
        }
        
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN: {
            Point position((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
            MouseButton button = MouseButton::Left;
            
            if (msg == WM_RBUTTONDOWN) button = MouseButton::Right;
            else if (msg == WM_MBUTTONDOWN) button = MouseButton::Middle;
            
            if (OnMouseEvent) {
                MouseEvent mouseEvent;
                mouseEvent.type = EventType::MouseButtonPressed;
                mouseEvent.position = position;
                mouseEvent.button = button;
                OnMouseEvent(mouseEvent);
            }
            
            if (rootWidget) {
                MouseEvent event;
                event.type = EventType::MouseButtonPressed;
                event.position = position;
                event.button = button;
                rootWidget->OnMouseEvent(event);
            }
            
            SetCapture(hwnd);
            return 0;
        }
        
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP: {
            Point position((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
            MouseButton button = MouseButton::Left;
            
            if (msg == WM_RBUTTONUP) button = MouseButton::Right;
            else if (msg == WM_MBUTTONUP) button = MouseButton::Middle;
            
            if (OnMouseEvent) {
                MouseEvent mouseEvent;
                mouseEvent.type = EventType::MouseButtonReleased;
                mouseEvent.position = position;
                mouseEvent.button = button;
                OnMouseEvent(mouseEvent);
            }
            
            if (rootWidget) {
                MouseEvent event;
                event.type = EventType::MouseButtonReleased;
                event.position = position;
                event.button = button;
                rootWidget->OnMouseEvent(event);
            }
            
            ReleaseCapture();
            return 0;
        }
        
        case WM_MOUSEMOVE: {
            Point position((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
            
            if (OnMouseEvent) {
                MouseEvent mouseEvent;
                mouseEvent.type = EventType::MouseMoved;
                mouseEvent.position = position;
                OnMouseEvent(mouseEvent);
            }
            
            if (rootWidget) {
                MouseEvent event;
                event.type = EventType::MouseMoved;
                event.position = position;
                rootWidget->OnMouseEvent(event);
            }
            
            return 0;
        }
        
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN: {
            KeyCode key = static_cast<KeyCode>(wParam);
            
            if (OnKeyEvent) {
                KeyEvent keyEvent;
                keyEvent.type = EventType::KeyPressed;
                keyEvent.keyCode = key;
                OnKeyEvent(keyEvent);
            }
            
            if (rootWidget) {
                KeyEvent event;
                event.type = EventType::KeyPressed;
                event.keyCode = key;
                rootWidget->OnKeyEvent(event);
            }
            
            return 0;
        }
        
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            KeyCode key = static_cast<KeyCode>(wParam);
            
            if (OnKeyEvent) {
                KeyEvent keyEvent;
                keyEvent.type = EventType::KeyReleased;
                keyEvent.keyCode = key;
                OnKeyEvent(keyEvent);
            }
            
            if (rootWidget) {
                KeyEvent event;
                event.type = EventType::KeyReleased;
                event.keyCode = key;
                rootWidget->OnKeyEvent(event);
            }
            
            return 0;
        }
        
        case WM_CHAR: {
            wchar_t character = static_cast<wchar_t>(wParam);
            
            if (rootWidget) {
                KeyEvent event;
                event.type = EventType::KeyTyped;
                event.character = character;
                rootWidget->OnKeyEvent(event);
            }
            
            return 0;
        }
        
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }
    
    return DefWindowProc(this->hwnd, msg, wParam, lParam);
}

// Factory function implementation
std::shared_ptr<Window> CreatePlatformWindow() {
    return std::make_shared<Win32Window>();
}

} // namespace miko