#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <Graphic/BufferObject.hpp>

class GRAPHIC_API VertexBuffer : public BufferObject {
   public:
    VertexBuffer();

    ~VertexBuffer();

    VertexBuffer(const VertexBuffer &other);

    VertexBuffer(VertexBuffer &&other);

    VertexBuffer &operator=(VertexBuffer other);

    friend void swap(VertexBuffer &first, VertexBuffer &second);

    bool initialize() override;

    template <typename T>
    void update(const T vertices, std::size_t cnt, GLenum type, GLenum mode);

    void drawPrimitive() const override;

   private:
    uint32_t m_VBO;
};

template <typename T>
void VertexBuffer::update(const T vertices, std::size_t cnt, GLenum type,
                          GLenum mode) {
    static_assert(std::is_pointer<T>::value, "Expected a pointer");
    bindVertexArray();
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
