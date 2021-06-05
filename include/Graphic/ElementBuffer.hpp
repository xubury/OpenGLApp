#ifndef VERTEX_ELEMENT_HPP
#define VERTEX_ELEMENT_HPP

#include "Graphic/BufferObject.hpp"

namespace te {

class TE_API ElementBuffer : public BufferObject {
   public:
    ElementBuffer();

    ~ElementBuffer();

    ElementBuffer(const ElementBuffer &other);

    ElementBuffer(ElementBuffer &&other);

    ElementBuffer &operator=(ElementBuffer other);

    friend void swap(ElementBuffer &first, ElementBuffer &second);

    void update(const Vertex *vertices, std::size_t vertexCnt,
                const uint32_t *indices, std::size_t indexCnt, GLenum type,
                GLenum mode);

    void update(const Vertex *vertices, std::size_t vertexCnt, GLenum type,
                GLenum mode);

    void drawPrimitive() const override;

   private:
    uint32_t m_VBO;
    uint32_t m_EBO;
};

}  // namespace te

#endif
