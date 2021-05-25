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
    VertexBuffer(int type);

    ~VertexBuffer();

    VertexBuffer(const VertexBuffer &) = delete;

    VertexBuffer &operator=(const VertexBuffer &) = delete;

    std::size_t size() const;

    bool empty() const;

    bool isInit() const;

    bool initialize();

    template <typename T>
    void update(const T vertices, std::size_t cnt,
                GLenum mode = GL_STATIC_DRAW);

    void draw(RenderTarget &target, RenderStates states) const override;

    void drawPrimitive() const;

   private:
    uint32_t m_VBO;
    uint32_t m_VAO;
    std::size_t m_size;
    int m_drawType;
};

template <typename T>
void VertexBuffer::update(const T vertices, std::size_t cnt, GLenum mode) {
    static_assert(std::is_pointer<T>::value, "Expected a pointer");
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(typename std::remove_pointer<T>::type) * cnt, vertices,
                 mode);
    m_size = cnt;
    std::remove_pointer<T>::type::setupAttribute();
}

#endif
