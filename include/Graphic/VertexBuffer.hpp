#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <Graphic/Export.hpp>
#include <Graphic/Vertex.hpp>
#include <Graphic/Drawable.hpp>

#include <glad/glad.h>
#include <stdint.h>
#include <iostream>
#include <vector>


typedef void (*AttrFunc)();

class GRAPHIC_API VertexBuffer : public Drawable {
   public:
    VertexBuffer();

    ~VertexBuffer();

    VertexBuffer(const VertexBuffer &other);

    VertexBuffer &operator=(VertexBuffer other);

    std::size_t size() const;

    bool empty() const;

    bool isInit() const;

    bool initialize();

    template <typename T>
    void update(const T vertices, std::size_t cnt, uint32_t type,
                uint32_t mode);

    void draw(RenderTarget &target, RenderStates states) const override;

    void drawPrimitive() const;

   private:
    uint32_t m_VBO;
    uint32_t m_VAO;
    std::size_t m_size;
    uint32_t m_mode;
    uint32_t m_primitiveType;
    AttrFunc m_attrFunction;
};

template <typename T>
void VertexBuffer::update(const T vertices, std::size_t cnt, uint32_t type,
                          uint32_t mode) {
    static_assert(std::is_pointer<T>::value, "Expected a pointer");
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(typename std::remove_pointer<T>::type) * cnt, vertices,
                 mode);
    m_size = cnt;
    m_primitiveType = type;
    m_attrFunction = &std::remove_pointer<T>::type::setupAttribute;
    m_attrFunction();
}

#endif
