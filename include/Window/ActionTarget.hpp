#ifndef ACTION_TARGET_HPP
#define ACTION_TARGET_HPP

#include <functional>
#include <list>
#include <utility>

#include <Window/ActionMap.hpp>

template <typename T>
class ActionTarget {
   public:
    using FuncType = std::function<void(const Event &)>;

    using KeyPair = std::pair<T, FuncType>;

    using ActionPair = std::pair<Action, FuncType>;

   public:
    ActionTarget(const ActionTarget<T> &) = delete;

    ActionTarget<T> &operator=(const ActionTarget<T> &) = delete;

    ActionTarget(const ActionMap<T> &map);

    bool processEvent(const Event &event) const;

    void processEvents() const;

    void bind(const T &key, const FuncType &callback);

    void bind(const Action &acton, const FuncType &callback);

    void bind(Action &&action, const FuncType &callback);

    void unbind(const T &key);

    void setActive(bool active);

   private:
    std::list<KeyPair> m_eventRealTime;
    std::list<KeyPair> m_eventPoll;
    std::list<ActionPair> m_eventRealTimeAction;
    std::list<ActionPair> m_eventPollAction;

    const ActionMap<T> &m_actionMap;

    bool m_active;
};

template <typename T>
ActionTarget<T>::ActionTarget(const ActionMap<T> &map)
    : m_actionMap(map), m_active(true) {}

template <typename T>
bool ActionTarget<T>::processEvent(const Event &event) const {
    if (!m_active) return false;

    for (const auto &[action, func] : m_eventPollAction) {
        if (action == event) {
            func(event);
            return true;
        }
    }
    for (const auto &[key, func] : m_eventPoll) {
        if (m_actionMap.get(key) == event) {
            func(event);
            return true;
        }
    }
    return false;
}

template <typename T>
void ActionTarget<T>::processEvents() const {
    if (!m_active) return;

    for (const auto &[action, func] : m_eventRealTimeAction) {
        if (action.test()) {
            func(action.m_event);
        }
    }
    for (const auto &[key, func] : m_eventRealTime) {
        const Action &action = m_actionMap.get(key);
        if (action.test()) {
            func(action.m_event);
        }
    }
}

template <typename T>
void ActionTarget<T>::bind(const T &key, const FuncType &callback) {
    const Action &action = m_actionMap.get(key);
    if (action.m_type & Action::Type::REAL_TIME) {
        m_eventRealTime.emplace_back(key, callback);
    } else {
        m_eventPoll.emplace_back(key, callback);
    }
}

template <typename T>
void ActionTarget<T>::bind(const Action &action, const FuncType &callback) {
    if (action.m_type & Action::Type::REAL_TIME) {
        m_eventRealTimeAction.emplace_back(action, callback);
    } else {
        m_eventPollAction.emplace_back(action, callback);
    }
}

template <typename T>
void ActionTarget<T>::bind(Action &&action, const FuncType &callback) {
    if (action.m_type & Action::Type::REAL_TIME) {
        m_eventRealTimeAction.emplace_back(std::move(action), callback);
    } else {
        m_eventPollAction.emplace_back(std::move(action), callback);
    }
}

template <typename T>
void ActionTarget<T>::unbind(const T &key) {
    const Action &action = m_actionMap.get(key);
    auto removeFunc = [&key](const ActionPair &pair) -> bool {
        return pair.first == key;
    };
    if (action.m_type & Action::Type::REAL_TIME) {
        m_eventRealTime.remove_if(removeFunc);
    } else {
        m_eventPoll.remove_if(removeFunc);
    }
}

template <typename T>
void ActionTarget<T>::setActive(bool active) {
    m_active = active;
}

#endif
