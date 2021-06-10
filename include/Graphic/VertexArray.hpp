#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include "Core/Base.hpp"
#include "Graphic/Buffer.hpp"

namespace te {

class VertexArray {
   public:
    VertexArray(GLenum type);

    ~VertexArray();

    VertexArray(const VertexArray &) = delete;

    VertexArray &operator=(const VertexArray &) = delete;

    void bind() const;

    void unbind() const;

    void addVertexBuffer(const Ref<VertexBuffer> &buffer);

    void setIndexBuffer(const Ref<IndexBuffer> &buffer);

    const std::vector<Ref<VertexBuffer>> &getVertexBuffers() const;

    const Ref<IndexBuffer> &getIndexBuffer() const;

    GLenum getType() const;

   private:
    uint32_t m_arrayId;
    uint32_t m_vertexBufferId;
    std::vector<Ref<VertexBuffer>> m_vertexBuffers;
    Ref<IndexBuffer> m_indexBuffer;
    GLenum m_type;
};

}  // namespace te

#endif /* VERTEX_ARRAY_HPP */
