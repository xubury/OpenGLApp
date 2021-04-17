#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <glad/glad.h>
#include <stdint.h>
#include <iostream>
#include <vector>

#include <Graphic/Vertex.hpp>
#include <Graphic/Drawable.hpp>

class VertexBuffer : public Drawable {
   public:
    VertexBuffer();

    ~VertexBuffer();

    VertexBuffer(const VertexBuffer &) = delete;

    VertexBuffer &operator=(const VertexBuffer &) = delete;

    std::size_t size() const;

    bool empty() const;

    template <typename T>
    bool create(const T vertices, std::size_t cnt);

    template <typename T>
    void update(const T vertices, std::size_t cnt);

    void draw(RenderTarget &target, RenderStates states) const override;

    void bind() const;

   private:
    uint32_t m_VBO;
    std::size_t m_size;
};

template <typename T>
bool VertexBuffer::create(const T vertices, std::size_t cnt) {
    static_assert(std::is_pointer<T>::value, "Expected a pointer");
    if (!m_VBO) glGenBuffers(1, &m_VBO);
    if (!m_VBO) {
        std::cerr << "Could not create vertex buffer." << std::endl;
        return false;
    }
    update(vertices, cnt);
    return true;
}

template <typename T>
void VertexBuffer::update(const T vertices, std::size_t cnt) {
    static_assert(std::is_pointer<T>::value, "Expected a pointer");
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(typename std::remove_pointer<T>::type) * cnt, vertices,
                 GL_STATIC_DRAW);
    m_size = cnt;
}

#endif
