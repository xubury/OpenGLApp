#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include <stdint.h>
#include <vector>

#include "Drawable.hpp"

class Vertex;

class VertexBuffer : public Drawable {
   public:
    VertexBuffer();

    std::size_t size() const;

    bool isAvailable() const;

    bool update(const std::vector<Vertex> &vertices);

    bool update(const Vertex *vertices, std::size_t cnt);

    void draw(RenderTarget &target) const;

    static void bind(const VertexBuffer *vertexBuffer);

   private:
    uint32_t m_VBO;
    std::size_t m_size;
};
#endif
