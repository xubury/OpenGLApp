#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <algorithm>
#include <bitset>
#include <list>
#include <tuple>
#include <vector>
#include <ECS/Component.hpp>
#include <ECS/System.hpp>
#include <Utility/Memory.hpp>

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

    EntityManager() : m_systems(nullptr){};
    ~EntityManager();

    void setSystems(SystemManager<ENTITY> *systems) { m_systems = systems; }
    SystemManager<ENTITY> *getSystems() const { return m_systems; }

    template <typename T = ENTITY, typename... ARGS>
    uint32_t create(ARGS &&...args);

    template <typename... ARGS>
    void emplace(uint32_t index, ARGS &&...args);

    void remove(std::size_t id);

    void update();

    void reset();

    std::size_t size();

    bool isValid(uint32_t id) const;

    const ENTITY &get(std::size_t id) const;
    ENTITY &get(std::size_t id);

    const ENTITY *getPtr(std::size_t id) const;
    ENTITY *getPtr(std::size_t id);

    Container::const_iterator begin() const;
    Container::const_iterator end() const;

    template <typename COMPONENT, typename... ARGS>
    ComponentHandle<COMPONENT, ENTITY> addComponent(uint32_t id,
                                                    ARGS &&...args);

    template <typename COMPONENT>
    void removeComponent(uint32_t id);

    template <typename COMPONENT>
    bool hasComponent(uint32_t id) const;

    template <typename COMPONENT>
    ComponentHandle<COMPONENT, ENTITY> getComponent(uint32_t id) const;

    template <typename... COMPONENT>
    std::tuple<ComponentHandle<COMPONENT, ENTITY>...> getComponents(
        uint32_t id) const;

    template <typename... COMPONENT>
    View<COMPONENT...> getByComponents(
        ComponentHandle<COMPONENT, ENTITY> &...components);

   private:
    SystemManager<ENTITY> *m_systems;
    // Stores every entity that have been allocated
    std::vector<ENTITY *> m_entites_allocated;
    // Stores what components does a entity of index has
    std::vector<std::bitset<MAX_COMPONENTS>> m_entities_component_masks;
    // Stores entities that has component of index
    std::vector<VPool *> m_components_entities;

    Container m_entities_index;
    Container m_entities_index_free;
    Container m_entities_to_destroy;

    void reset(uint32_t id);

    template <typename COMPONENT>
    void checkComponent();

    template <typename COMPONENT>
    COMPONENT *getComponentPtr(uint32_t id) const;

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
            ENTITY *operator*() const;
            ENTITY *operator->() const;

            bool operator==(const Iterator &other) const;
            bool operator!=(const Iterator &other) const;

           private:
            View &m_view;
            EntityManager<ENTITY>::Container::iterator m_iter;
            EntityManager<ENTITY>::Container::iterator m_iter_end;
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
    if (!m_entities_index_free.empty()) {
        index = m_entities_index_free.front();
        m_entities_index_free.pop_front();
        m_entites_allocated[index] =
            new T(this, index, std::forward<ARGS>(args)...);
    } else {
        m_entities_component_masks.emplace_back();

        index = m_entites_allocated.size();
        m_entites_allocated.emplace_back(nullptr);

        std::size_t comp_size = m_components_entities.size();
        for (std::size_t i = 0; i < comp_size; ++i) {
            if (m_components_entities[i] != nullptr) {
                m_components_entities[i]->resize(index + 1);
            }
        }
        m_entites_allocated[index] =
            new T(this, index, std::forward<ARGS>(args)...);
    }
    m_entities_index.emplace_back(index);
    return index;
}

template <class ENTITY>
template <typename... ARGS>
inline void EntityManager<ENTITY>::emplace(uint32_t index, ARGS &&...args) {
    std::size_t size = m_entites_allocated.size();
    if (size <= index) {
        m_entites_allocated.resize(index + 1, nullptr);
        m_entities_component_masks.resize(index + 1, 0);
        m_entities_index.emplace_back(index);

        for (std::size_t i = size; i < index; ++i) {
            m_entities_index_free.emplace_back(i);
        }

        std::size_t comp_size = m_components_entities.size();
        for (std::size_t i = 0; i < comp_size; ++i) {
            if (m_components_entities[i] != nullptr) {
                /* resize the components pool in case the new entity needs to
                 * add a certain kind of component */
                m_components_entities[i]->resize(index + 1);
            }
        }
    } else if (m_entites_allocated[index] !=
               nullptr) {  // already in use, free it
        reset(index);
    } else {
        m_entities_index_free.remove(index);
        m_entities_index.emplace_back(index);
    }

    m_entites_allocated[index] =
        new ENTITY(this, index, std::forward<ARGS>(args)...);
}

template <class ENTITY>
inline void EntityManager<ENTITY>::remove(std::size_t id) {
    m_entities_to_destroy.emplace_back(id);
}

template <class ENTITY>
inline void EntityManager<ENTITY>::update() {
    if (!m_entities_to_destroy.empty()) {
        Container::iterator iter = m_entities_to_destroy.begin();
        Container::iterator end = m_entities_to_destroy.end();
        for (; iter != end; ++iter) {
            uint32_t id = *iter;
            ENTITY *entity = m_entites_allocated.at(id);
            if (entity != nullptr) {
                reset(id);
                m_entities_index.erase(std::find(m_entities_index.begin(),
                                                 m_entities_index.end(), id));
            }
        }
        m_entities_to_destroy.clear();
    }
}

template <class ENTITY>
inline void EntityManager<ENTITY>::reset() {
    m_entities_index_free.clear();
    m_entities_index.clear();

    std::size_t comp_size = m_components_entities.size();
    for (std::size_t i = 0; i < comp_size; ++i) {
        if (m_components_entities[i] != nullptr) {
            for (std::size_t j = 0; j < m_entities_index.size(); ++j) {
                m_components_entities[i]->erase<VComponent<ENTITY>>(j);
            }
            delete m_components_entities[i];
        }
    }
    m_components_entities.clear();
    m_entities_component_masks.clear();
    std::size_t size = m_entites_allocated.size();
    for (std::size_t i = 0; i < size; ++i) {
        delete m_entites_allocated[i];
    }
    m_entites_allocated.clear();
}

template <class ENTITY>
inline std::size_t EntityManager<ENTITY>::size() {
    return m_entites_allocated.size() - m_entities_index_free.size();
}

template <class ENTITY>
inline bool EntityManager<ENTITY>::isValid(uint32_t id) const {
    return id < m_entites_allocated.size() &&
           m_entites_allocated[id] != nullptr;
}
template <class ENTITY>
inline const ENTITY &EntityManager<ENTITY>::get(std::size_t id) const {
    return *m_entites_allocated.at(id);
}

template <class ENTITY>
inline ENTITY &EntityManager<ENTITY>::get(std::size_t id) {
    return *m_entites_allocated.at(id);
}

template <class ENTITY>
inline const ENTITY *EntityManager<ENTITY>::getPtr(std::size_t id) const {
    return m_entites_allocated.at(id);
}

template <class ENTITY>
inline ENTITY *EntityManager<ENTITY>::getPtr(std::size_t id) {
    return m_entites_allocated.at(id);
}

template <class ENTITY>
inline EntityManager<ENTITY>::Container::const_iterator
EntityManager<ENTITY>::begin() const {
    return m_entities_index.cbegin();
}

template <class ENTITY>
inline EntityManager<ENTITY>::Container::const_iterator
EntityManager<ENTITY>::end() const {
    return m_entities_index.cend();
}

template <class ENTITY>
template <typename COMPONENT, typename... ARGS>
inline ComponentHandle<COMPONENT, ENTITY> EntityManager<ENTITY>::addComponent(
    uint32_t id, ARGS &&...args) {
    checkComponent<COMPONENT>();
    uint32_t family = COMPONENT::family();

    assert(!m_entities_component_masks.at(id).test(family));
    Pool<COMPONENT> *pool =
        static_cast<Pool<COMPONENT> *>(m_components_entities[family]);

    pool->emplace(id, std::forward<ARGS>(args)...);
    pool->at(id).m_owner_id = id;
    pool->at(id).m_manager = this;

    m_entities_component_masks.at(id).set(family);
    return ComponentHandle<COMPONENT, ENTITY>(this, id);
}

template <class ENTITY>
template <typename COMPONENT>
inline void EntityManager<ENTITY>::removeComponent(uint32_t id) {
    // checkComponent<COMPONENT>();
    uint32_t family = COMPONENT::family();
    assert(m_entities_component_masks.at(id).test(family));

    Pool<COMPONENT> *pool =
        static_cast<Pool<COMPONENT> *>(m_components_entities[family]);
    pool->erase(id);

    m_entities_component_masks.at(id).reset(family);
}

template <class ENTITY>
template <typename COMPONENT>
inline bool EntityManager<ENTITY>::hasComponent(uint32_t id) const {
    uint32_t family = COMPONENT::family();
    return m_entities_component_masks.at(id).test(family);
}

template <class ENTITY>
template <typename COMPONENT>
inline ComponentHandle<COMPONENT, ENTITY> EntityManager<ENTITY>::getComponent(
    uint32_t id) const {
    if (hasComponent<COMPONENT>(id)) {
        return ComponentHandle<COMPONENT, ENTITY>(this, id);
    }
    return ComponentHandle<COMPONENT, ENTITY>();
}

template <class ENTITY>
template <typename... COMPONENT>
inline std::tuple<ComponentHandle<COMPONENT, ENTITY>...>
EntityManager<ENTITY>::getComponents(uint32_t id) const {
    return std::make_tuple(getComponent<COMPONENT>(id)...);
}

template <class ENTITY>
template <typename COMPONENT>
inline COMPONENT *EntityManager<ENTITY>::getComponentPtr(uint32_t id) const {
    uint32_t family = COMPONENT::family();
    return &static_cast<Pool<COMPONENT> *>(m_components_entities[family])
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
    ENTITY *entity = m_entites_allocated.at(id);
    std::size_t comp_size = m_components_entities.size();
    for (std::size_t i = 0; i < comp_size; ++i) {
        if (m_components_entities[i] != nullptr) {
            m_components_entities[i]->erase<VComponent<ENTITY>>(id);
        }
    }
    m_entities_component_masks.at(id).reset();
    delete entity;
    m_entites_allocated[id] = nullptr;
}

template <class ENTITY>
template <typename COMPONENT>
inline void EntityManager<ENTITY>::checkComponent() {
    uint32_t family = COMPONENT::family();
    if (m_components_entities.size() <= family) {
        m_components_entities.resize(family + 1, nullptr);
    }

    if (m_components_entities[family] == nullptr) {
        Pool<COMPONENT> *pool = new Pool<COMPONENT>;
        pool->resize(m_entites_allocated.size());
        m_components_entities[family] = pool;
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
    Container::iterator begin = m_manager.m_entities_index.begin();
    Container::iterator end = m_manager.m_entities_index.end();
    for (; begin != end; ++begin) {
        uint32_t index = *begin;
        if ((m_manager.m_entities_component_masks[index] & m_mask) == m_mask) {
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
    return Iterator(*this, m_manager.m_entities_index.end(),
                    m_manager.m_entities_index.end());
}

template <class ENTITY>
template <typename... COMPONENT>
template <int N, typename C>
inline void EntityManager<ENTITY>::View<COMPONENT...>::unpackId(
    std::uint32_t id) {
    std::get<N>(m_handles).m_entity_id = id;
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
    EntityManager<ENTITY>::Container::iterator it_end)
    : m_view(view), m_iter(it), m_iter_end(it_end) {}

template <class ENTITY>
template <typename... COMPONENT>
typename EntityManager<ENTITY>::template View<COMPONENT...>::Iterator &
EntityManager<ENTITY>::View<COMPONENT...>::Iterator::operator++() {
    ++m_iter;
    while (m_iter != m_iter_end) {
        std::uint32_t index = *m_iter;
        if (m_view.m_manager.m_entites_allocated.at(index) != nullptr &&
            (m_view.m_manager.m_entities_component_masks.at(index) &
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
inline ENTITY *EntityManager<ENTITY>::View<COMPONENT...>::Iterator::operator*()
    const {
    if (m_iter == m_iter_end) return nullptr;
    return m_view.m_manager.m_entites_allocated.at(*m_iter);
}

template <class ENTITY>
template <typename... COMPONENT>
inline ENTITY *EntityManager<ENTITY>::View<COMPONENT...>::Iterator::operator->()
    const {
    if (m_iter == m_iter_end) return nullptr;
    return m_view.m_manager.m_entites_allocated.at(*m_iter);
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

#endif
