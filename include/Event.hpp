#ifndef EVENT_HPP
#define EVENT_HPP

#include "Keyboard.hpp"
#include "Mouse.hpp"

class Event {
   public:
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
        KeyEvent key;
        MouseMoveEvent mouseMove;
        MouseButtonEvent mouseButton;
        MouseWheelEvent mouseWheel;
    };

    enum EventType {
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
