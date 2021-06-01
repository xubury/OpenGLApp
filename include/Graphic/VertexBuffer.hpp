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

    void update(const Vertex *vertices, std::size_t cnt, GLenum type,
                GLenum mode);

    void drawPrimitive() const override;

   private:
    uint32_t m_VBO;
};

#endif
