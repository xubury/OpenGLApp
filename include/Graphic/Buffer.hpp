#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>
#include <stdint.h>
#include "Core/Assert.hpp"
#include "Graphic/Layout.hpp"
#include "Graphic/OpenGL.hpp"

namespace te {

class VertexBuffer {
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

class IndexBuffer {
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

class UniformBuffer {
   public:
    UniformBuffer(std::size_t size);

    ~UniformBuffer();

    UniformBuffer(const UniformBuffer &) = delete;

    UniformBuffer &operator=(const UniformBuffer &) = delete;

    void clearData();

    void setData(const void *data, std::size_t size);

    void bind() const;

    void unbind() const;

    uint32_t getBindingPoint() const;
   private:
    uint32_t m_bufferId;
    std::size_t m_size;
    uint32_t m_slot;
    uint32_t m_offset;
    static uint32_t s_count;
};

}  // namespace te

#endif /* GL_BUFFER_HPP */
