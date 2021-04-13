#include "Action.hpp"

Action::Action(const Event::EventType &event) : m_type(Type::Pressed) {
    m_event.type = event;
}

Action::Action(const Keyboard::Key &key, int type) : m_type(type) {
    m_event.key.code = key;
    m_event.type = Event::EventType::KeyPressed;
}

bool Action::operator==(const Event &event) const {
    bool res = false;
    // TODO: add code for handling other event
    switch (event.type) {
        case Event::EventType::KeyPressed: {
            if (m_type & Type::Pressed &&
                m_event.type == Event::EventType::KeyPressed)
                res = event.key.code == m_event.key.code;
        } break;
        case Event::EventType::KeyReleased: {
            if (m_type & Type::Released &&
                m_event.type == Event::EventType::KeyPressed)
                res = event.key.code == m_event.key.code;
        } break;
        default:
            res = m_event.type == event.type;
            break;
    }
    return res;
}

bool Action::test() const {
    bool res = false;
    // TODO:other detect (mouse joystick) need to be implemented
    if (m_event.type == Event::EventType::KeyPressed) {
        if (m_type & Type::Pressed) {
            res = Keyboard::isKeyPressed(m_event.key.code);
        }
    }
    return res;
}
