#pragma once

#ifndef MIKO_EVENT_H
#define MIKO_EVENT_H

#include "Math.h"
#include <functional>
#include <vector>
#include <memory>

namespace miko {

    enum class EventType {
        None = 0,
        
        // Window events
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        
        // Mouse events
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,
        MouseEntered,
        MouseLeft,
        
        // Keyboard events
        KeyPressed,
        KeyReleased,
        KeyTyped,
        
        // Widget events
        WidgetClicked,
        WidgetHover,
        WidgetFocusGained,
        WidgetFocusLost,
        WidgetValueChanged
    };

    enum class MouseButton {
        Left = 0,
        Right = 1,
        Middle = 2
    };

    enum class KeyCode {
        Unknown = 0,
        
        // Letters
        A = 65, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        
        // Numbers
        Num0 = 48, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
        
        // Function keys
        F1 = 112, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        
        // Special keys
        Space = 32,
        Enter = 13,
        Tab = 9,
        Backspace = 8,
        Delete = 46,
        Escape = 27,
        
        // Arrow keys
        Left = 37,
        Up = 38,
        Right = 39,
        Down = 40,
        
        // Modifier keys
        Shift = 16,
        Control = 17,
        Alt = 18
    };

    struct Event {
        EventType type = EventType::None;
        bool handled = false;
        
        virtual ~Event() = default;
    };

    struct WindowEvent : public Event {
        int width = 0;
        int height = 0;
    };

    struct MouseEvent : public Event {
        Point position;
        MouseButton button = MouseButton::Left;
        float wheelDelta = 0.0f;
        bool ctrlPressed = false;
        bool shiftPressed = false;
        bool altPressed = false;
    };

    struct KeyEvent : public Event {
        KeyCode keyCode = KeyCode::Unknown;
        char character = 0;
        bool ctrlPressed = false;
        bool shiftPressed = false;
        bool altPressed = false;
        bool repeat = false;
    };

    struct WidgetEvent : public Event {
        class Widget* widget = nullptr;
        Point position;
    };

    // Event handler function type
    template<typename T>
    using EventHandler = std::function<void(const T&)>;

    // Event dispatcher
    class EventDispatcher {
    public:
        template<typename T>
        void Subscribe(EventHandler<T> handler) {
            static_assert(std::is_base_of_v<Event, T>, "T must derive from Event");
            // Implementation will be in the source file
        }
        
        template<typename T>
        void Dispatch(const T& event) {
            static_assert(std::is_base_of_v<Event, T>, "T must derive from Event");
            // Implementation will be in the source file
        }
        
        void Clear();
        
    private:
        // Implementation details will be in the source file
        struct HandlerBase {
            virtual ~HandlerBase() = default;
        };
        
        template<typename T>
        struct TypedHandler : public HandlerBase {
            EventHandler<T> handler;
            TypedHandler(EventHandler<T> h) : handler(std::move(h)) {}
        };
        
        std::vector<std::unique_ptr<HandlerBase>> handlers;
    };

    // Global event dispatcher
    EventDispatcher& GetEventDispatcher();

} // namespace miko

#endif // MIKO_EVENT_H