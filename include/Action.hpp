#ifndef ACTION_HPP
#define ACTION_HPP

#include "Event.hpp"

class Action {
   public:
    enum Type { RealTime = 1, Pressed = 1 << 1, Released = 1 << 2 };

    Action(const Event::EventType &event);

    Action(Keyboard::Key key, int type = Type::RealTime | Type::Pressed);

    Action(Mouse::Button button, int type = Type::RealTime | Type::Pressed);

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
