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
        int x;
        int y;
    };

    struct MouseButtonEvent {
        Mouse::Button button;
        int x;
        int y;
        bool alt;
        bool control;
        bool shift;
        bool system;
    };

    union {
        KeyEvent key;
        MouseMoveEvent mouseMove;
        MouseButtonEvent mouseButton;
    };

    enum EventType {
        KeyPressed,
        KeyReleased,
        MouseMoved,
        MouseButtonPressed,
        MouseButtonReleased
    };

    EventType type;
};

#endif
