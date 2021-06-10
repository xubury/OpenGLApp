#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <Graphic/Drawable.hpp>
#include <string>

#include "ECS/System.hpp"
#include "ECS/EntityManager.hpp"
#include "ECS/Component.hpp"

namespace te {

template <typename ENTITY>
class EntityManager;
template <typename COMPONENT, typename ENTITY>
class Component;
template <typename COMPONENT, typename ENTITY>
class ComponentHandle;

template <typename ENTITY>
class Entity {
   public:
    Entity(Entity &&) = default;

    Entity &operator=(Entity &&) = default;

    Entity(EntityManager<ENTITY> *manager, uint32_t id);

    uint32_t id() const;

    EntityManager<ENTITY> &getManager();

    bool operator==(const Entity &other) const;

    bool operator!=(const Entity &other) const;

    void remove();

    template <typename COMPONENT, typename... ARGS>
    void add(ARGS &&...args);

    template <typename COMPONENT>
    void remove();

    template <typename COMPONENT>
    bool has() const;

    template <typename COMPONENT>
    ComponentHandle<COMPONENT, ENTITY> component();

    template <typename COMPONENT>
    const ComponentHandle<COMPONENT, ENTITY> component() const;

    template <typename... COMPONENT>
    std::tuple<ComponentHandle<COMPONENT, ENTITY>...> components();

   private:
    uint32_t m_id;
    EntityManager<ENTITY> *m_manager;
};

#define ES_INIT_ENTITY(ENTITY) \
    ES_INIT_VCOMPONENT(ENTITY) \
    ES_INIT_VSYSTEM(ENTITY)

template <typename ENTITY>
Entity<ENTITY>::Entity(EntityManager<ENTITY> *manager, uint32_t id)
    : m_id(id), m_manager(manager) {}

template <typename ENTITY>
inline uint32_t Entity<ENTITY>::id() const {
    return m_id;
}

template <typename ENTITY>
inline EntityManager<ENTITY> &Entity<ENTITY>::getManager() {
    return *m_manager;
}

template <typename ENTITY>
bool Entity<ENTITY>::operator==(const Entity<ENTITY> &other) const {
    return m_id == other.m_id && m_manager == other.m_manager;
}

template <typename ENTITY>
bool Entity<ENTITY>::operator!=(const Entity<ENTITY> &other) const {
    return m_id != other.m_id || m_manager != other.m_manager;
}

template <typename ENTITY>
inline void Entity<ENTITY>::remove() {
    m_manager->remove(m_id);
}

template <typename ENTITY>
template <typename COMPONENT, typename... Args>
inline void Entity<ENTITY>::add(Args &&...args) {
    m_manager->template addComponent<COMPONENT>(m_id,
                                                std::forward<Args>(args)...);
}

template <typename ENTITY>
template <typename COMPONENT>
inline void Entity<ENTITY>::remove() {
    m_manager->template removeComponent<COMPONENT>(m_id);
}

template <typename ENTITY>
template <typename COMPONENT>
inline bool Entity<ENTITY>::has() const {
    return m_manager->template hasComponent<COMPONENT>(m_id);
}

template <typename ENTITY>
template <typename COMPONENT>
inline ComponentHandle<COMPONENT, ENTITY> Entity<ENTITY>::component() {
    return m_manager->template getComponent<COMPONENT>(m_id);
}

template <typename ENTITY>
template <typename COMPONENT>
inline const ComponentHandle<COMPONENT, ENTITY> Entity<ENTITY>::component() const {
    return m_manager->template getComponent<COMPONENT>(m_id);
}

template <typename ENTITY>
template <typename... COMPONENT>
inline std::tuple<ComponentHandle<COMPONENT, ENTITY>...>
Entity<ENTITY>::components() {
    return m_manager->template getComponents<COMPONENT...>(m_id);
}

}  // namespace te

#endif
