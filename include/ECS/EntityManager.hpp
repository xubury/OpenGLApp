#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <algorithm>
#include <bitset>
#include <list>
#include <tuple>
#include <vector>
#include "ECS/Defines.hpp"
#include "ECS/Component.hpp"
#include "ECS/System.hpp"
#include "Core/Memory.hpp"

namespace te {

template <typename COMPONENT, typename ENTITY>
class ComponentHandle;

template <typename COMPONENT, typename ENTITY>
class Component;

template <class ENTITY>
class EntityManager {
   private:
    template <typename...>
    class View;

    template <typename, typename>
    friend class ComponentHandle;

   public:
    using Container = std::list<uint32_t>;

   public:
    EntityManager(const EntityManager &) = delete;
    EntityManager &operator=(const EntityManager &) = delete;

    EntityManager() = default;

    ~EntityManager();

    template <typename T = ENTITY, typename... ARGS>
    uint32_t create(ARGS &&...args);

    template <typename... ARGS>
    void emplace(uint32_t index, ARGS &&...args);

    void remove(std::size_t id);

    void update(const Time &deltaTime);

    void reset();

    std::size_t size();

    bool isValid(uint32_t id) const;

    const ENTITY *get(std::size_t id) const;

    ENTITY *get(std::size_t id);

    Container::const_iterator cbegin() const;

    Container::const_iterator cend() const;

    Container::iterator begin();

    Container::iterator end();

    template <typename COMPONENT, typename... ARGS>
    ComponentHandle<COMPONENT, ENTITY> addComponent(uint32_t id,
                                                    ARGS &&...args);

    template <typename COMPONENT>
    void removeComponent(uint32_t id);

    template <typename COMPONENT>
    bool hasComponent(uint32_t id) const;

    template <typename COMPONENT>
    ComponentHandle<COMPONENT, ENTITY> getComponent(uint32_t id);

    template <typename COMPONENT>
    const ComponentHandle<COMPONENT, ENTITY> getComponent(uint32_t id) const;

    template <typename... COMPONENT>
    std::tuple<ComponentHandle<COMPONENT, ENTITY>...> getComponents(
        uint32_t id);

    template <typename... COMPONENT>
    View<COMPONENT...> getByComponents(
        ComponentHandle<COMPONENT, ENTITY> &...components);

   private:
    // Stores every entity that have been allocated
    std::vector<Scope<ENTITY>> m_entitiesAllocated;
    // Stores what components does a entity of index has
    std::vector<std::bitset<MAX_COMPONENTS>> m_entitiesComponentMasks;
    // Stores entities that has component of index
    std::vector<Scope<VPool>> m_componentsEntities;

    Container m_entitiesIndex;
    Container m_entitiesIndexFree;
    Container m_entitiesToDestroy;

    void reset(uint32_t id);

    template <typename COMPONENT>
    void checkComponent();

    template <typename COMPONENT>
    COMPONENT *getComponentPtr(uint32_t id);

    template <typename COMPONENT>
    const COMPONENT *getComponentPtr(uint32_t id) const;

    template <typename... COMPONENT>
    class View {
        class Iterator;

       public:
        View(EntityManager<ENTITY> &manager,
             const std::bitset<MAX_COMPONENTS> &mask,
             ComponentHandle<COMPONENT, ENTITY> &...components);
        Iterator begin();
        Iterator end();

       private:
        template <int N, typename C>
        void unpackId(std::uint32_t id);

        template <int N, typename C0, typename C1, typename... Cx>
        void unpackId(std::uint32_t id);

        template <int N, typename C>
        void unpackManager();

        template <int N, typename C0, typename C1, typename... Cx>
        void unpackManager();

        EntityManager<ENTITY> &m_manager;
        const std::bitset<MAX_COMPONENTS> m_mask;
        std::tuple<ComponentHandle<COMPONENT, ENTITY> &...> m_handles;

       private:
        class Iterator {
           public:
            Iterator(View &view, EntityManager<ENTITY>::Container::iterator it,
                     EntityManager<ENTITY>::Container::iterator it_end);
            Iterator &operator++();
            const ENTITY &operator*() const;
            const ENTITY *operator->() const;

            ENTITY &operator*();
            ENTITY *operator->();

            bool operator==(const Iterator &other) const;
            bool operator!=(const Iterator &other) const;

           private:
            View &m_view;
            EntityManager<ENTITY>::Container::iterator m_iter;
            EntityManager<ENTITY>::Container::iterator m_iterEnd;
        };
    };
};

template <class ENTITY>
EntityManager<ENTITY>::~EntityManager() {
    reset();
}

template <class ENTITY>
template <typename T, typename... ARGS>
inline uint32_t EntityManager<ENTITY>::create(ARGS &&...args) {
    static_assert(std::is_base_of<ENTITY, T>::value,
                  "T must be derived from ENTITY");
    uint32_t index = 0;
    if (!m_entitiesIndexFree.empty()) {
        index = m_entitiesIndexFree.front();
        m_entitiesIndexFree.pop_front();
        m_entitiesAllocated[index] =
            createScope<T>(this, index, std::forward<ARGS>(args)...);
    } else {
        m_entitiesComponentMasks.emplace_back();

        index = m_entitiesAllocated.size();
        m_entitiesAllocated.emplace_back(nullptr);

        std::size_t comp_size = m_componentsEntities.size();
        for (std::size_t i = 0; i < comp_size; ++i) {
            if (m_componentsEntities[i] != nullptr) {
                m_componentsEntities[i]->resize(index + 1);
            }
        }
        m_entitiesAllocated[index] =
            createScope<T>(this, index, std::forward<ARGS>(args)...);
    }
    m_entitiesIndex.emplace_back(index);
    return index;
}

template <class ENTITY>
template <typename... ARGS>
inline void EntityManager<ENTITY>::emplace(uint32_t index, ARGS &&...args) {
    std::size_t size = m_entitiesAllocated.size();
    if (size <= index) {
        m_entitiesAllocated.resize(index + 1);
        m_entitiesComponentMasks.resize(index + 1, 0);
        m_entitiesIndex.emplace_back(index);

        for (std::size_t i = size; i < index; ++i) {
            m_entitiesIndexFree.emplace_back(i);
        }

        std::size_t compSize = m_componentsEntities.size();
        for (std::size_t i = 0; i < compSize; ++i) {
            if (m_componentsEntities[i] != nullptr) {
                /* resize the components pool in case the new entity needs to
                 * add a certain kind of component */
                m_componentsEntities[i]->resize(index + 1);
            }
        }
    } else if (m_entitiesAllocated[index] != nullptr) {
        // already in use, free it
        reset(index);
    } else {
        m_entitiesIndexFree.remove(index);
        m_entitiesIndex.emplace_back(index);
    }

    m_entitiesAllocated[index] =
        createScope<ENTITY>(this, index, std::forward<ARGS>(args)...);
}

template <class ENTITY>
inline void EntityManager<ENTITY>::remove(std::size_t id) {
    m_entitiesToDestroy.emplace_back(id);
}

template <class ENTITY>
inline void EntityManager<ENTITY>::update(const Time &deltaTime) {
    if (!m_entitiesToDestroy.empty()) {
        Container::iterator iter = m_entitiesToDestroy.begin();
        Container::iterator end = m_entitiesToDestroy.end();
        for (; iter != end; ++iter) {
            uint32_t id = *iter;
            if (m_entitiesAllocated.at(id) != nullptr) {
                reset(id);
                m_entitiesIndex.erase(std::find(m_entitiesIndex.begin(),
                                                m_entitiesIndex.end(), id));
            }
        }
        m_entitiesToDestroy.clear();
    }
    for (auto &entity : m_entitiesAllocated) {
        entity->update(deltaTime);
    }
}

template <class ENTITY>
inline void EntityManager<ENTITY>::reset() {
    m_entitiesIndexFree.clear();
    m_entitiesIndex.clear();

    std::size_t comp_size = m_componentsEntities.size();
    for (std::size_t i = 0; i < comp_size; ++i) {
        if (m_componentsEntities[i] != nullptr) {
            for (std::size_t j = 0; j < m_entitiesIndex.size(); ++j) {
                m_componentsEntities[i]->erase<VComponent<ENTITY>>(j);
            }
        }
    }
    m_componentsEntities.clear();
    m_entitiesComponentMasks.clear();
    m_entitiesAllocated.clear();
}

template <class ENTITY>
inline std::size_t EntityManager<ENTITY>::size() {
    return m_entitiesAllocated.size() - m_entitiesIndexFree.size();
}

template <class ENTITY>
inline bool EntityManager<ENTITY>::isValid(uint32_t id) const {
    return id < m_entitiesAllocated.size() &&
           m_entitiesAllocated[id] != nullptr;
}

template <class ENTITY>
inline const ENTITY *EntityManager<ENTITY>::get(std::size_t id) const {
    return m_entitiesAllocated.at(id).get();
}

template <class ENTITY>
inline ENTITY *EntityManager<ENTITY>::get(std::size_t id) {
    return m_entitiesAllocated.at(id).get();
}

template <class ENTITY>
inline EntityManager<ENTITY>::Container::const_iterator
EntityManager<ENTITY>::cbegin() const {
    return m_entitiesIndex.cbegin();
}

template <class ENTITY>
inline EntityManager<ENTITY>::Container::const_iterator
EntityManager<ENTITY>::cend() const {
    return m_entitiesIndex.cend();
}

template <class ENTITY>
inline EntityManager<ENTITY>::Container::iterator
EntityManager<ENTITY>::begin() {
    return m_entitiesIndex.begin();
}

template <class ENTITY>
inline EntityManager<ENTITY>::Container::iterator EntityManager<ENTITY>::end() {
    return m_entitiesIndex.end();
}

template <class ENTITY>
template <typename COMPONENT, typename... ARGS>
inline ComponentHandle<COMPONENT, ENTITY> EntityManager<ENTITY>::addComponent(
    uint32_t id, ARGS &&...args) {
    checkComponent<COMPONENT>();
    uint32_t family = COMPONENT::family();

    TE_CORE_ASSERT(!m_entitiesComponentMasks.at(id).test(family),
                   "EntityManager::addComponent component already exists!");
    Pool<COMPONENT> *pool =
        static_cast<Pool<COMPONENT> *>(m_componentsEntities[family].get());

    pool->emplace(id, std::forward<ARGS>(args)...);
    pool->at(id).m_ownerID = id;
    pool->at(id).m_manager = this;

    m_entitiesComponentMasks.at(id).set(family);
    return ComponentHandle<COMPONENT, ENTITY>(this, id);
}

template <class ENTITY>
template <typename COMPONENT>
inline void EntityManager<ENTITY>::removeComponent(uint32_t id) {
    // checkComponent<COMPONENT>();
    uint32_t family = COMPONENT::family();
    TE_CORE_ASSERT(m_entitiesComponentMasks.at(id).test(family),
                   "EntityManager::removeComponent component not exists!");

    Pool<COMPONENT> *pool =
        static_cast<Pool<COMPONENT> *>(m_componentsEntities[family].get());
    pool->erase(id);

    m_entitiesComponentMasks.at(id).reset(family);
}

template <class ENTITY>
template <typename COMPONENT>
inline bool EntityManager<ENTITY>::hasComponent(uint32_t id) const {
    uint32_t family = COMPONENT::family();
    return m_entitiesComponentMasks.at(id).test(family);
}

template <class ENTITY>
template <typename COMPONENT>
inline ComponentHandle<COMPONENT, ENTITY> EntityManager<ENTITY>::getComponent(
    uint32_t id) {
    if (hasComponent<COMPONENT>(id)) {
        return ComponentHandle<COMPONENT, ENTITY>(this, id);
    }
    return ComponentHandle<COMPONENT, ENTITY>();
}

template <class ENTITY>
template <typename COMPONENT>
inline const ComponentHandle<COMPONENT, ENTITY>
EntityManager<ENTITY>::getComponent(uint32_t id) const {
    if (hasComponent<COMPONENT>(id)) {
        return ComponentHandle<COMPONENT, ENTITY>(this, id);
    }
    return ComponentHandle<COMPONENT, ENTITY>();
}

template <class ENTITY>
template <typename... COMPONENT>
inline std::tuple<ComponentHandle<COMPONENT, ENTITY>...>
EntityManager<ENTITY>::getComponents(uint32_t id) {
    return std::make_tuple(getComponent<COMPONENT>(id)...);
}

template <class ENTITY>
template <typename COMPONENT>
inline COMPONENT *EntityManager<ENTITY>::getComponentPtr(uint32_t id) {
    uint32_t family = COMPONENT::family();
    TE_CORE_ASSERT(id < m_componentsEntities[family].get()->size(),
                   "Component id invalid!");
    return &static_cast<Pool<COMPONENT> *>(m_componentsEntities[family].get())
                ->at(id);
}

template <class ENTITY>
template <typename COMPONENT>
inline const COMPONENT *EntityManager<ENTITY>::getComponentPtr(
    uint32_t id) const {
    uint32_t family = COMPONENT::family();
    TE_CORE_ASSERT(id < m_componentsEntities[family].get()->size(),
                   "Component id invalid!");
    return &static_cast<Pool<COMPONENT> *>(m_componentsEntities[family].get())
                ->at(id);
}

template <typename COMPONENT>
inline void getMask(std::bitset<MAX_COMPONENTS> &mask) {
    mask.set(COMPONENT::family());
}

template <typename C1, typename C2, typename... COMPONENT>
inline void getMask(std::bitset<MAX_COMPONENTS> &mask) {
    mask.set(C1::family());
    getMask<C2, COMPONENT...>(mask);
}

template <class ENTITY>
template <typename... COMPONENT>
inline typename EntityManager<ENTITY>::template View<COMPONENT...>
EntityManager<ENTITY>::getByComponents(
    ComponentHandle<COMPONENT, ENTITY> &...components) {
    std::bitset<MAX_COMPONENTS> mask;
    getMask<COMPONENT...>(mask);
    return View<COMPONENT...>(*this, mask, components...);
}

template <class ENTITY>
inline void EntityManager<ENTITY>::reset(uint32_t id) {
    std::size_t comp_size = m_componentsEntities.size();
    for (std::size_t i = 0; i < comp_size; ++i) {
        if (m_componentsEntities[i] != nullptr) {
            m_componentsEntities[i]->erase<VComponent<ENTITY>>(id);
        }
    }
    m_entitiesComponentMasks.at(id).reset();
    m_entitiesAllocated[id] = nullptr;
}

template <class ENTITY>
template <typename COMPONENT>
inline void EntityManager<ENTITY>::checkComponent() {
    uint32_t family = COMPONENT::family();
    if (m_componentsEntities.size() <= family) {
        m_componentsEntities.resize(family + 1);
    }

    if (m_componentsEntities[family] == nullptr) {
        Scope<Pool<COMPONENT>> pool = createScope<Pool<COMPONENT>>();
        pool->resize(m_entitiesAllocated.size());
        m_componentsEntities[family] = std::move(pool);
    }
}

template <class ENTITY>
template <typename... COMPONENT>
EntityManager<ENTITY>::View<COMPONENT...>::View(
    EntityManager<ENTITY> &manager, const std::bitset<MAX_COMPONENTS> &mask,
    ComponentHandle<COMPONENT, ENTITY> &...components)
    : m_manager(manager),
      m_mask(mask),
      m_handles(
          std::tuple<ComponentHandle<COMPONENT, ENTITY> &...>(components...)) {
    unpackManager<0, COMPONENT...>();
}

template <class ENTITY>
template <typename... COMPONENT>
inline typename EntityManager<ENTITY>::template View<COMPONENT...>::Iterator
EntityManager<ENTITY>::View<COMPONENT...>::begin() {
    Container::iterator begin = m_manager.m_entitiesIndex.begin();
    Container::iterator end = m_manager.m_entitiesIndex.end();
    for (; begin != end; ++begin) {
        uint32_t index = *begin;
        if ((m_manager.m_entitiesComponentMasks[index] & m_mask) == m_mask) {
            unpackId<0, COMPONENT...>(index);
            break;
        }
    }
    return Iterator(*this, begin, end);
}

template <class ENTITY>
template <typename... COMPONENT>
inline typename EntityManager<ENTITY>::template View<COMPONENT...>::Iterator
EntityManager<ENTITY>::View<COMPONENT...>::end() {
    return Iterator(*this, m_manager.m_entitiesIndex.end(),
                    m_manager.m_entitiesIndex.end());
}

template <class ENTITY>
template <typename... COMPONENT>
template <int N, typename C>
inline void EntityManager<ENTITY>::View<COMPONENT...>::unpackId(
    std::uint32_t id) {
    std::get<N>(m_handles).m_entityID = id;
}

template <class ENTITY>
template <typename... COMPONENT>
template <int N, typename C0, typename C1, typename... Cx>
inline void EntityManager<ENTITY>::View<COMPONENT...>::unpackId(
    std::uint32_t id) {
    unpackId<N, C0>(id);
    unpackId<N + 1, C1, Cx...>(id);
}

template <class ENTITY>
template <typename... COMPONENT>
template <int N, typename C>
inline void EntityManager<ENTITY>::View<COMPONENT...>::unpackManager() {
    std::get<N>(m_handles).m_manager = &m_manager;
}

template <class ENTITY>
template <typename... COMPONENT>
template <int N, typename C0, typename C1, typename... Cx>
inline void EntityManager<ENTITY>::View<COMPONENT...>::unpackManager() {
    unpackManager<N, C0>();
    unpackManager<N + 1, C1, Cx...>();
}

template <class ENTITY>
template <typename... COMPONENT>
EntityManager<ENTITY>::View<COMPONENT...>::Iterator::Iterator(
    View &view, EntityManager<ENTITY>::Container::iterator it,
    EntityManager<ENTITY>::Container::iterator itEnd)
    : m_view(view), m_iter(it), m_iterEnd(itEnd) {}

template <class ENTITY>
template <typename... COMPONENT>
typename EntityManager<ENTITY>::template View<COMPONENT...>::Iterator &
EntityManager<ENTITY>::View<COMPONENT...>::Iterator::operator++() {
    ++m_iter;
    while (m_iter != m_iterEnd) {
        std::uint32_t index = *m_iter;
        if (m_view.m_manager.m_entitiesAllocated.at(index) != nullptr &&
            (m_view.m_manager.m_entitiesComponentMasks.at(index) &
             m_view.m_mask) == m_view.m_mask) {
            m_view.unpackId<0, COMPONENT...>(index);
            break;
        }
        ++m_iter;
    }
    return *this;
}

template <class ENTITY>
template <typename... COMPONENT>
inline const ENTITY &
EntityManager<ENTITY>::View<COMPONENT...>::Iterator::operator*() const {
    if (m_iter == m_iterEnd) return nullptr;
    return *m_view.m_manager.m_entitiesAllocated.at(*m_iter).get();
}

template <class ENTITY>
template <typename... COMPONENT>
inline const ENTITY *
EntityManager<ENTITY>::View<COMPONENT...>::Iterator::operator->() const {
    if (m_iter == m_iterEnd) return nullptr;
    return m_view.m_manager.m_entitiesAllocated.at(*m_iter).get();
}

template <class ENTITY>
template <typename... COMPONENT>
inline ENTITY &
EntityManager<ENTITY>::View<COMPONENT...>::Iterator::operator*() {
    if (m_iter == m_iterEnd) return nullptr;
    return *m_view.m_manager.m_entitiesAllocated.at(*m_iter).get();
}

template <class ENTITY>
template <typename... COMPONENT>
inline ENTITY *
EntityManager<ENTITY>::View<COMPONENT...>::Iterator::operator->() {
    if (m_iter == m_iterEnd) return nullptr;
    return m_view.m_manager.m_entitiesAllocated.at(*m_iter).get();
}

template <class ENTITY>
template <typename... COMPONENT>
bool EntityManager<ENTITY>::View<COMPONENT...>::Iterator::operator==(
    const EntityManager<ENTITY>::View<COMPONENT...>::Iterator &other) const {
    return m_iter == other.m_iter && m_view.m_mask == other.m_view.m_mask &&
           &(m_view.m_manager) == &(other.m_view.m_manager);
}

template <class ENTITY>
template <typename... COMPONENT>
bool EntityManager<ENTITY>::View<COMPONENT...>::Iterator::operator!=(
    const EntityManager<ENTITY>::View<COMPONENT...>::Iterator &other) const {
    return m_iter != other.m_iter || m_view.m_mask != other.m_view.m_mask ||
           &(m_view.m_manager) != &(other.m_view.m_manager);
}

}  // namespace te

#endif
