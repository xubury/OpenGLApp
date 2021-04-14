#ifndef ACTION_HPP
#define ACTION_HPP

#include "Window/Event.hpp"

class Action {
   public:
    enum Type { REAL_TIME = 1, PRESSED = 1 << 1, RELEASED = 1 << 2 };

    Action(const Event::EventType &event);

    Action(Keyboard::Key key, int type = Type::REAL_TIME | Type::PRESSED);

    Action(Mouse::Button button, int type = Type::REAL_TIME | Type::PRESSED);

    bool test() const;

    bool operator==(const Event &event) const;

    bool operator==(const Action &other) const;

   private:
    template <typename>
    friend class ActionTarget;

    Event m_event;
    int m_type;
};

#endif
