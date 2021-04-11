#ifndef RESOURCE_MANAGER
#define RESOURCE_MANAGER

#include <stdexcept>
#include <unordered_map>
#include <memory>

template <typename RESOURCE, typename IDENTIFIER>
class ResourceManager {
   public:
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;

    ResourceManager() = default;

    template <typename... ARGS>
    RESOURCE &load(const IDENTIFIER &id, ARGS &&...args);

    bool count(const IDENTIFIER &id);

    RESOURCE &get(const IDENTIFIER &id);

    template <typename... ARGS>
    RESOURCE &getOrLoad(const IDENTIFIER &id, ARGS &&...args);

    void clear();

   private:
    std::unordered_map<IDENTIFIER, std::unique_ptr<RESOURCE>> m_map;
};

template <typename RESOURCE, typename IDENTIFIER>
template <typename... ARGS>
RESOURCE &ResourceManager<RESOURCE, IDENTIFIER>::load(const IDENTIFIER &id,
                                                      ARGS &&...args) {
    std::unique_ptr<RESOURCE> ptr(new RESOURCE);
    if (!ptr->loadFromFile(std::forward<ARGS>(args)...)) {
        throw std::runtime_error("Cannot load from file");
    }
    if (m_map.emplace(id, std::move(ptr)).second == false) {
        throw std::runtime_error(
            "Fail to implace in map, object already loaded.");
    }
    return *m_map[id];
}

template <typename RESOURCE, typename IDENTIFIER>
RESOURCE &ResourceManager<RESOURCE, IDENTIFIER>::get(const IDENTIFIER &id) {
    return *m_map.at(id);
}

template <typename RESOURCE, typename IDENTIFIER>
bool ResourceManager<RESOURCE, IDENTIFIER>::count(const IDENTIFIER &id) {
    return m_map.count(id);
}

template <typename RESOURCE, typename IDENTIFIER>
template <typename... ARGS>
RESOURCE &ResourceManager<RESOURCE, IDENTIFIER>::getOrLoad(const IDENTIFIER &id,
                                                           ARGS &&...args) {
    if (m_map.count(id) == 0) {
        return load(id, std::forward<ARGS>(args)...);
    }
    return get(id);
}

template <typename RESOURCE, typename IDENTIFIER>
void ResourceManager<RESOURCE, IDENTIFIER>::clear() {
    m_map.clear();
}

#endif
