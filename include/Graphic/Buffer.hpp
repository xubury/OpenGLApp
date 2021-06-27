#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>
#include <stdint.h>
#include "Core/Export.hpp"
#include "Core/Assert.hpp"
#include "Graphic/Layout.hpp"
#include "Graphic/OpenGL.hpp"

namespace te {

class TE_API VertexBuffer {
   public:
    VertexBuffer(const void *vertices, std::size_t size);

    ~VertexBuffer();

    VertexBuffer(const VertexBuffer &) = delete;

    VertexBuffer &operator=(const VertexBuffer &) = delete;

    void bind() const;

    void unbind() const;

    const BufferLayout &getLayout() const;

    void setLayout(const BufferLayout &layout);

    std::size_t getSize() const;

    void update(const void *vertices, std::size_t size);

   private:
    uint32_t m_bufferId;
    BufferLayout m_layout;
    std::size_t m_size;
};

class TE_API IndexBuffer {
   public:
    IndexBuffer(const uint32_t *indices, std::size_t count);

    ~IndexBuffer();

    IndexBuffer(const IndexBuffer &) = delete;

    IndexBuffer &operator=(const IndexBuffer &) = delete;

    void bind() const;

    void unbind() const;

    std::size_t getCount() const;

    void update(const uint32_t *indices, std::size_t count);

   private:
    uint32_t m_bufferId;
    std::size_t m_count;
};

class TE_API UniformBuffer {
   public:
    UniformBuffer(std::size_t size);

    ~UniformBuffer();

    UniformBuffer(const UniformBuffer &) = delete;

    UniformBuffer &operator=(const UniformBuffer &) = delete;

    void setData(const void *data, std::size_t offset, std::size_t size);

    void bind() const;

    void unbind() const;

    uint32_t getBindingPoint() const;
   private:
    uint32_t m_bufferId;
    std::size_t m_size;
    uint32_t m_slot;
    static uint32_t s_count;
};

}  // namespace te

#endif /* GL_BUFFER_HPP */
