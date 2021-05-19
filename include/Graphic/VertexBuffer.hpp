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

    template <typename T>
    bool create(const T vertices, std::size_t cnt);

    template <typename T>
    void update(const T vertices, std::size_t cnt);

    void draw(RenderTarget &target, RenderStates states) const override;

    void bind() const;

    void drawPrimitive() const;

   private:
    uint32_t m_VBO;
    uint32_t m_VAO;
    std::size_t m_size;
    int m_drawType;
};

template <typename T>
bool VertexBuffer::create(const T vertices, std::size_t cnt) {
    glGenBuffers(1, &m_VBO);
    glGenVertexArrays(1, &m_VAO);
    if (!m_VBO || !m_VAO) {
        std::cerr << "Could not create vertex buffer." << std::endl;
        return false;
    }
    if (!m_VAO) {
        std::cerr << "Could not create vertex array object." << std::endl;
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
