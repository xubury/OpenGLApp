#include "Window/Action.hpp"

namespace te {

Action::Action(const Event::EventType &event) : m_type(Type::PRESSED) {
    m_event.type = event;
}

Action::Action(Keyboard::Key key, int type) : m_type(type) {
    m_event.key.code = key;
    m_event.type = Event::EventType::KEY_PRESSED;
}

Action::Action(Mouse::Button button, int type) : m_type(type) {
    m_event.mouseButton.button = button;
    m_event.type = Event::EventType::MOUSE_BUTTON_PRESSED;
}

bool Action::operator==(const Event &event) const {
    bool res = false;
    // TODO: add code for handling other event
    switch (event.type) {
        case Event::EventType::KEY_PRESSED: {
            if (m_type & Type::PRESSED &&
                m_event.type == Event::EventType::KEY_PRESSED)
                res = event.key.code == m_event.key.code;
        } break;
        case Event::EventType::KEY_RELEASED: {
            if (m_type & Type::RELEASED &&
                m_event.type == Event::EventType::KEY_PRESSED)
                res = event.key.code == m_event.key.code;
        } break;
        case Event::EventType::MOUSE_BUTTON_PRESSED: {
            if (m_type & Type::PRESSED &&
                m_event.type == Event::EventType::MOUSE_BUTTON_PRESSED)
                res = event.mouseButton.button == m_event.mouseButton.button;
        } break;
        case Event::EventType::MOUSE_BUTTON_RELEASED: {
            if (m_type & Type::RELEASED &&
                m_event.type == Event::EventType::MOUSE_BUTTON_RELEASED)
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
    if (m_event.type == Event::EventType::KEY_PRESSED) {
        if (m_type & Type::PRESSED) {
            res = Keyboard::isKeyPressed(m_event.key.code);
        }
    } else if (m_event.type == Event::EventType::MOUSE_BUTTON_PRESSED) {
        if (m_type & Type::PRESSED) {
            res = Mouse::isButtonPressed(m_event.mouseButton.button);
        }
    }
    return res;
}

}  // namespace te
