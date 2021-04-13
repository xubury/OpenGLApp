#include "Action.hpp"

Action::Action(const Event::EventType &event) : m_type(Type::Pressed) {
    m_event.type = event;
}

Action::Action(Keyboard::Key key, int type) : m_type(type) {
    m_event.key.code = key;
    m_event.type = Event::EventType::KeyPressed;
}

Action::Action(Mouse::Button button, int type) : m_type(type) {
    m_event.mouseButton.button = button;
    m_event.type = Event::EventType::MouseButtonPressed;
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
        case Event::EventType::MouseButtonPressed: {
            if (m_type & Type::Pressed &&
                m_event.type == Event::EventType::MouseButtonPressed)
                res = event.mouseButton.button == m_event.mouseButton.button;
        } break;
        case Event::EventType::MouseButtonReleased: {
            if (m_type & Type::Released &&
                m_event.type == Event::EventType::MouseButtonReleased)
                res = event.mouseButton.button == m_event.mouseButton.button;
        } break;
        default:
            res = m_event.type == event.type;
            break;
    }
    return res;
}

bool Action::test() const {
    bool res = false;
    // TODO:other detect (joystick) need to be implemented
    if (m_event.type == Event::EventType::KeyPressed) {
        if (m_type & Type::Pressed) {
            res = Keyboard::isKeyPressed(m_event.key.code);
        }
    } else if (m_event.type == Event::EventType::MouseButtonPressed) {
        if (m_type & Type::Pressed) {
            res = Mouse::isButtonPressed(m_event.mouseButton.button);
        }
    }
    return res;
}
