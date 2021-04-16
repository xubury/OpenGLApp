#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include <stdint.h>
#include <vector>

#include <Graphic/Drawable.hpp>

class Vertex;

class VertexBuffer : public Drawable {
   public:
    VertexBuffer();

    ~VertexBuffer();

    VertexBuffer(const VertexBuffer &) = delete;

    VertexBuffer &operator=(const VertexBuffer &) = delete;

    std::size_t size() const;

    bool empty() const;

    bool create(const Vertex *vertices, std::size_t cnt);

    void update(const Vertex *vertices, std::size_t cnt);

    void draw(RenderTarget &target, RenderStates states) const override;

    static void bind(const VertexBuffer *vertexBuffer);

   private:
    uint32_t m_VBO;
    std::size_t m_size;
};

#endif
