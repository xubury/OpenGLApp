#ifndef GL_RESOURCE_HPP
#define GL_RESOURCE_HPP

#include <stdint.h>
#include <utility>

class GlResource {
   protected:
    template <typename>
    friend class GlBuffer;

    GlResource() = default;

    GlResource(const GlResource &) = delete;

    GlResource &operator=(const GlResource &) = delete;

    virtual void destroy() = 0;

    virtual ~GlResource() = default;
};

template <typename T>
class GlBuffer {
   public:
    explicit GlBuffer(T *ptr = nullptr);

    ~GlBuffer();

    GlBuffer(const GlBuffer &);

    GlBuffer &operator=(const GlBuffer &) = delete;

    uint64_t useCount();

    T *get();

    T &operator*();

    const T &operator*() const;

    T *operator->();

   private:
    T *m_resource;
    uint64_t *m_counter;
};

template <typename T>
GlBuffer<T>::GlBuffer(T *ptr) {
    m_resource = ptr;
    m_counter = new uint64_t(0);
    if (m_resource) {
        ++(*m_counter);
    }
}

template <typename T>
GlBuffer<T>::~GlBuffer() {
    --(*m_counter);
    if (*m_counter == 0) {
        delete m_counter;
        dynamic_cast<GlResource *>(m_resource)->destroy();
        delete m_resource;
    }
}

template <typename T>
GlBuffer<T>::GlBuffer(const GlBuffer &other)
    : m_resource(other.m_resource), m_counter(other.m_counter) {
    ++(*m_counter);
}

template <typename T>
uint64_t GlBuffer<T>::useCount() {
    return *m_counter;
}

template <typename T>
T *GlBuffer<T>::get() {
    return m_resource;
}
template <typename T>
T *GlBuffer<T>::operator->() {
    return m_resource;
}

template <typename T>
T &GlBuffer<T>::operator*() {
    return *m_resource;
}

template <typename T>
const T &GlBuffer<T>::operator*() const {
    return *m_resource;
}

#endif
