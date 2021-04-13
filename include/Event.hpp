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
        KeyPressed,
        KeyReleased,
        MouseMoved,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseWheelScrolled
    };

    EventType type;
};

#endif
