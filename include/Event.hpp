#ifndef EVENT_HPP
#define EVENT_HPP

class Event {
   public:
    struct KeyEvent {
        int code;
        bool alt;
        bool control;
        bool shift;
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
