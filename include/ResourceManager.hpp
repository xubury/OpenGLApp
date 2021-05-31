#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER

#include <stdexcept>
#include <unordered_map>
#include <memory>

template <typename IDENTIFIER, typename RESOURCE>
class ResourceManager {
    using Ptr = std::shared_ptr<RESOURCE>;

   public:
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;

    ResourceManager() = default;

    template <typename... ARGS>
    Ptr load(const IDENTIFIER &id, ARGS &&...args);

    bool count(const IDENTIFIER &id);

    Ptr get(const IDENTIFIER &id);

    template <typename... ARGS>
    Ptr getOrLoad(const IDENTIFIER &id, ARGS &&...args);

    void clear();

   private:
    std::unordered_map<IDENTIFIER, Ptr> m_map;
};

template <typename IDENTIFIER, typename RESOURCE>
template <typename... ARGS>
typename ResourceManager<IDENTIFIER, RESOURCE>::Ptr
ResourceManager<IDENTIFIER, RESOURCE>::load(const IDENTIFIER &id,
                                            ARGS &&...args) {
    std::shared_ptr<RESOURCE> ptr(new RESOURCE);
    if (!ptr->load(std::forward<ARGS>(args)...)) {
        throw std::runtime_error("Cannot load from file");
    }
    if (m_map.emplace(id, std::move(ptr)).second == false) {
        throw std::runtime_error(
            "Fail to implace in map, object already loaded.");
    }
    return m_map[id];
}

template <typename IDENTIFIER, typename RESOURCE>
typename ResourceManager<IDENTIFIER, RESOURCE>::Ptr
ResourceManager<IDENTIFIER, RESOURCE>::get(const IDENTIFIER &id) {
    return m_map.at(id);
}

template <typename IDENTIFIER, typename RESOURCE>
bool ResourceManager<IDENTIFIER, RESOURCE>::count(const IDENTIFIER &id) {
    return m_map.count(id);
}

template <typename IDENTIFIER, typename RESOURCE>
template <typename... ARGS>
typename ResourceManager<IDENTIFIER, RESOURCE>::Ptr
ResourceManager<IDENTIFIER, RESOURCE>::getOrLoad(const IDENTIFIER &id,
                                                 ARGS &&...args) {
    if (m_map.count(id) == 0) {
        return load(id, std::forward<ARGS>(args)...);
    }
    return get(id);
}

template <typename IDENTIFIER, typename RESOURCE>
void ResourceManager<IDENTIFIER, RESOURCE>::clear() {
    m_map.clear();
}

#endif
