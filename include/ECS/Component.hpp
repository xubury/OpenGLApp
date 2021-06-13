#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "ECS/Defines.hpp"

namespace te {

template <typename ENTITY>
class EntityManager;

template <typename COMPONENT, typename ENTITY>
class ComponentHandle {
   public:
    ComponentHandle(const ComponentHandle &) = default;
    ComponentHandle &operator=(const ComponentHandle &) = default;

    ComponentHandle();

    bool isValid() const;

    COMPONENT *get();

    COMPONENT *operator->();

    const COMPONENT *get() const;

    const COMPONENT *operator->() const;

   private:
    friend class EntityManager<ENTITY>;

    ComponentHandle(EntityManager<ENTITY> *manager, uint32_t entity_id);

    EntityManager<ENTITY> *m_manager;
    uint32_t m_entityID;
};

template <typename ENTITY>
class VComponent {
   public:
    virtual ~VComponent() = default;

    uint32_t ownerID() const;

    ENTITY *owner();

    const ENTITY *owner() const;

    EntityManager<ENTITY> *manager() { return m_manager; }

   protected:
    friend class EntityManager<ENTITY>;

    VComponent();

    static uint32_t s_familyCounter;

   private:
    EntityManager<ENTITY> *m_manager;
    uint32_t m_ownerID;
};

#define ES_INIT_VCOMPONENT(ENTITY) \
    template <>                    \
    uint32_t VComponent<ENTITY>::s_familyCounter = 0;

template <typename COMPONENT, typename ENTITY>
class Component : public VComponent<ENTITY> {
   public:
    Component(const Component &) = delete;
    Component &operator=(const Component &) = delete;

    Component() = default;
    virtual ~Component() = default;

    void remove();
    static uint32_t family();
    using Handle = ComponentHandle<COMPONENT, ENTITY>;
};

template <typename COMPONENT, typename ENTITY>
inline bool ComponentHandle<COMPONENT, ENTITY>::isValid() const {
    return m_manager != nullptr && m_entityID != INVALID_ENTITY_ID;
}

template <typename COMPONENT, typename ENTITY>
ComponentHandle<COMPONENT, ENTITY>::ComponentHandle()
    : m_manager(nullptr), m_entityID(INVALID_ENTITY_ID) {}

template <typename COMPONENT, typename ENTITY>
ComponentHandle<COMPONENT, ENTITY>::ComponentHandle(
    EntityManager<ENTITY> *manager, uint32_t entity_id)
    : m_manager(manager), m_entityID(entity_id) {}

template <typename COMPONENT, typename ENTITY>
inline COMPONENT *ComponentHandle<COMPONENT, ENTITY>::get() {
    TE_CORE_ASSERT(isValid(), "manager not valid!");
    return m_manager->template getComponentPtr<COMPONENT>(m_entityID);
}

template <typename COMPONENT, typename ENTITY>
inline COMPONENT *ComponentHandle<COMPONENT, ENTITY>::operator->() {
    TE_CORE_ASSERT(isValid(), "manager not valid!");
    return m_manager->template getComponentPtr<COMPONENT>(m_entityID);
}

template <typename COMPONENT, typename ENTITY>
inline const COMPONENT *ComponentHandle<COMPONENT, ENTITY>::get() const {
    TE_CORE_ASSERT(isValid(), "manager not valid!");
    return m_manager->template getComponentPtr<COMPONENT>(m_entityID);
}

template <typename COMPONENT, typename ENTITY>
inline const COMPONENT *ComponentHandle<COMPONENT, ENTITY>::operator->() const {
    TE_CORE_ASSERT(isValid(), "manager not valid!");
    return m_manager->template getComponentPtr<COMPONENT>(m_entityID);
}

template <typename ENTITY>
VComponent<ENTITY>::VComponent() : m_manager(nullptr), m_ownerID(-1) {}

template <typename ENTITY>
inline uint32_t VComponent<ENTITY>::ownerID() const {
    return m_ownerID;
}

template <typename ENTITY>
inline ENTITY *VComponent<ENTITY>::owner() {
    return m_manager->get(m_ownerID);
}

template <typename ENTITY>
inline const ENTITY *VComponent<ENTITY>::owner() const {
    return m_manager->get(m_ownerID);
}

template <typename COMPONENT, typename ENTITY>
inline void Component<COMPONENT, ENTITY>::remove() {
    VComponent<ENTITY>::m_manager->template removeComponent<COMPONENT>(
        VComponent<ENTITY>::m_ownerID);
}

template <typename COMPONENT, typename ENTITY>
inline uint32_t Component<COMPONENT, ENTITY>::family() {
    static uint32_t family = VComponent<ENTITY>::s_familyCounter++;
    TE_CORE_ASSERT(family < MAX_COMPONENTS, "components size reach maximum");
    return family;
}

}  // namespace te

#endif
