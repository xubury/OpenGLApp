#ifndef ACTION_MAP_HPP
#define ACTION_MAP_HPP

#include <unordered_map>
#include "Window/Action.hpp"

namespace te {

template <typename T = int>
class ActionMap {
   public:
    ActionMap() = default;

    ActionMap(const ActionMap<T> &) = delete;

    ActionMap<T> &operator=(const ActionMap<T> &) = delete;

    void map(const T &key, const Action &action);

    const Action &get(const T &key) const;

   private:
    std::unordered_map<T, Action> m_map;
};

template <typename T>
void ActionMap<T>::map(const T &key, const Action &action) {
    m_map.emplace(key, action);
}

template <typename T>
const Action &ActionMap<T>::get(const T &key) const {
    return m_map.at(key);
}

}


#endif
