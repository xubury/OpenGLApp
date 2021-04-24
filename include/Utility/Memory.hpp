#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <vector>

class VPool {
   public:
    VPool(const VPool&) = delete;
    VPool& operator=(const VPool&) = delete;

    VPool(VPool&&) = default;
    VPool& operator=(VPool&&) = default;

    VPool() = default;
    virtual ~VPool() = default;

    std::size_t size() const;
    void resize(std::size_t size);

    bool isSet(std::size_t index) const;

    template <typename T>
    T& at(std::size_t index) const;

    template <typename T>
    void erase(std::size_t index);

    template <typename T, typename... ARGS>
    void emplace(std::size_t index, ARGS&&... args);

    template <typename T>
    class Iterator {
       public:
        Iterator(std::size_t index, const VPool& pool);

        bool operator!=(const Iterator<T>& other);
        void operator++();
        T& operator*() const;
        T& data() const;
        std::size_t index() const;

       private:
        std::size_t m_index;
        const VPool& m_pool;
    };

   protected:
    template <typename T>
    friend class Iterator;
    std::vector<void*> m_memory;
};

template <typename T>
class Pool : public VPool {
   public:
    Pool(const Pool&) = delete;
    Pool& operator=(const Pool&) = delete;

    Pool(Pool&&) = default;
    Pool& operator=(Pool&&) = default;

    Pool() = default;
    virtual ~Pool();

    T& at(std::size_t index) const;

    void erase(std::size_t index);

    template <typename... ARGS>
    void emplace(std::size_t index, ARGS&&... args);

    VPool::Iterator<T> Begin() const;
    VPool::Iterator<T> End() const;
};

template <typename T>
T& VPool::at(std::size_t index) const {
    return *static_cast<T*>(m_memory.at(index));
}

template <typename T>
void VPool::erase(std::size_t index) {
    delete static_cast<T*>(m_memory.at(index));
    m_memory[index] = nullptr;
}

template <typename T, typename... Args>
void VPool::emplace(std::size_t index, Args&&... args) {
    delete static_cast<T*>(m_memory.at(index));
    m_memory[index] = new T(args...);
}

template <typename T>
VPool::Iterator<T>::Iterator(std::size_t index, const VPool& pool)
    : m_index(index), m_pool(pool) {}

template <typename T>
bool VPool::Iterator<T>::operator!=(const Iterator<T>& other) {
    return m_index != other._index || &m_pool != &other._pool;
}

template <typename T>
void VPool::Iterator<T>::operator++() {
    auto size = m_pool.m_memory.size();
    ++m_index;
    while (m_index < size || m_pool.m_memory[m_index] == nullptr) ++m_index;
}

template <typename T>
T& VPool::Iterator<T>::operator*() const {
    return m_pool.at<T>(m_index);
}

template <typename T>
T& VPool::Iterator<T>::data() const {
    return m_pool.at<T>(m_index);
}

template <typename T>
std::size_t VPool::Iterator<T>::index() const {
    return m_index;
}

template <typename T>
T& Pool<T>::at(std::size_t index) const {
    return VPool::at<T>(index);
}

template <typename T>
void Pool<T>::erase(std::size_t index) {
    VPool::erase<T>(index);
}

template <typename T>
template <typename... Args>
void Pool<T>::emplace(std::size_t index, Args&&... args) {
    VPool::emplace<T>(index, args...);
}

template <typename T>
Pool<T>::~Pool() {
    const std::size_t size = m_memory.size();
    for (std::size_t i = 0; i < size; ++i) delete static_cast<T*>(m_memory[i]);
    m_memory.clear();
}

template <typename T>
VPool::Iterator<T> Pool<T>::Begin() const {
    std::size_t index = 0;
    std::size_t size = m_memory.size();

    while (index < size and m_memory[index] == nullptr) ++index;

    return VPool::Iterator<T>(index, *this);
}

template <typename T>
VPool::Iterator<T> Pool<T>::End() const {
    return VPool::Iterator<T>(m_memory.size(), *this);
}

#endif
