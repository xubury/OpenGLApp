#ifndef EVENT_HPP
#define EVENT_HPP

#include "Window/Keyboard.hpp"
#include "Window/Mouse.hpp"

class Event {
   public:
    struct SizeEvent {
        uint32_t width;
        uint32_t height;
    };

    struct KeyEvent {
        Keyboard::Key code;
        bool alt;
        bool control;
        bool shift;
        bool system;
    };

    struct MouseMoveEvent {
        double x;
        double y;
    };

    struct MouseButtonEvent {
        Mouse::Button button;
        double x;
        double y;
        bool alt;
        bool control;
        bool shift;
        bool system;
    };

    struct MouseWheelEvent {
        double xOffset;
        double yOffset;
        double x;
        double y;
    };

    union {
        SizeEvent size;
        KeyEvent key;
        MouseMoveEvent mouseMove;
        MouseButtonEvent mouseButton;
        MouseWheelEvent mouseWheel;
    };

    enum EventType {
        RESIZED,
        KEY_PRESSED,
        KEY_RElEASED,
        MOUSE_MOVED,
        MOUSE_BUTTON_PRESSED,
        MOUSE_BUTTON_RELEASED,
        MOUSE_WHEEL_SCROLLED
    };

    EventType type;
};

#endif
