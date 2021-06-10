#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "Core/Time.hpp"
#include "ECS/Defines.hpp"
#include <memory>
#include <unordered_map>

namespace te {

template <typename ENTITY>
class EntityManager;
template <typename ENTITY>
class Entity;

template <typename ENTITY>
class VSystem {
   public:
    VSystem(const VSystem&) = delete;
    VSystem& operator=(const VSystem&) = delete;

    virtual ~VSystem() = default;

    virtual void update(EntityManager<ENTITY>& entity_manager,
                        const Time& deltaTime) = 0;

   protected:
    VSystem() = default;
    static uint32_t s_familyCounter;
};

template <typename COMPONENT, typename ENTITY>
class System : public VSystem<ENTITY> {
   public:
    System(const System&) = delete;
    System& operator=(const System&) = delete;

    System() = default;
    virtual ~System() = default;

    static uint32_t family();
};

#define ES_INIT_VSYSTEM(ENTITY) \
    template <>                 \
    uint32_t VSystem<ENTITY>::s_familyCounter = 0;

template <typename ENTITY>
class SystemManager {
   public:
    SystemManager(const SystemManager&) = delete;
    SystemManager& operator=(const SystemManager&) = delete;

    SystemManager(EntityManager<ENTITY>& manager);
    ~SystemManager() = default;

    template <typename SYSTEM>
    bool add(std::shared_ptr<SYSTEM> ptr);

    template <typename SYSTEM, typename... Args>
    bool add(Args&&... args);

    template <typename SYSTEM>
    bool Remove();

    template <typename SYSTEM>
    SYSTEM* system();

    template <typename SYSTEM>
    void update(const Time& deltaTime);

    void updateAll(const Time& deltaTime);

   private:
    EntityManager<ENTITY>& m_manager;
    std::unordered_map<uint32_t, std::shared_ptr<VSystem<ENTITY>>> m_systems;
};

template <typename COMPONENT, typename ENTITY>
uint32_t System<COMPONENT, ENTITY>::family() {
    static uint32_t family = VSystem<ENTITY>::s_familyCounter++;
    TE_CORE_ASSERT(family < MAX_COMPONENTS, "components size reach maximum");
    return family;
}

template <typename ENTITY>
SystemManager<ENTITY>::SystemManager(EntityManager<ENTITY>& manager)
    : m_manager(manager) {}

template <typename ENTITY>
void SystemManager<ENTITY>::updateAll(const Time& deltaTime) {
    for (auto& [id, system] : m_systems) system->update(m_manager, deltaTime);
}

template <typename ENTITY>
template <typename SYSTEM>
bool SystemManager<ENTITY>::add(std::shared_ptr<SYSTEM> ptr) {
    if (m_systems.count(SYSTEM::family()) == 0) {
        m_systems.insert(std::make_pair(SYSTEM::family(), ptr));
        return true;
    }
    return false;
}

template <typename ENTITY>
template <typename SYSTEM, typename... Args>
bool SystemManager<ENTITY>::add(Args&&... args) {
    if (m_systems.count(SYSTEM::family()) == 0) {
        m_systems.emplace(
            SYSTEM::family(),
            std::shared_ptr<SYSTEM>(new SYSTEM(std::forward<Args>(args)...)));
        return true;
    }
    return false;
}

template <typename ENTITY>
template <typename SYSTEM>
bool SystemManager<ENTITY>::Remove() {
    if (m_systems.count(SYSTEM::family()) == 0) return false;
    m_systems.erase(SYSTEM::family());
    return true;
}

template <typename ENTITY>
template <typename SYSTEM>
inline SYSTEM* SystemManager<ENTITY>::system() {
    return std::static_pointer_cast<SYSTEM>(m_systems.at(SYSTEM::family()))
        .get();
}

template <typename ENTITY>
template <typename SYSTEM>
inline void SystemManager<ENTITY>::update(const Time& deltaTime) {
    system<SYSTEM>()->update(m_manager, deltaTime);
}

}  // namespace te

#endif
