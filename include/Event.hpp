#ifndef EVENT_HPP
#define EVENT_HPP

#include "Keyboard.hpp"

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
    union {
        KeyEvent key;
        MouseMoveEvent mouseMove;
    };

    enum EventType { KeyPressed, KeyReleased, MouseMoved };

    EventType type;
};

#endif
