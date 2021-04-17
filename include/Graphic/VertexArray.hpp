#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <stdint.h>
#include <vector>

#include <Graphic/Vertex.hpp>
#include <Graphic/Drawable.hpp>

class VertexArray : public Drawable {
   public:
    VertexArray();

    ~VertexArray();

    VertexArray(const VertexArray &) = delete;

    VertexArray &operator=(const VertexArray &) = delete;

    std::size_t size() const;

    bool empty() const;

    bool create(const Vertex *vertices, std::size_t cnt);

    void update(const Vertex *vertices, std::size_t cnt);

    void draw(RenderTarget &target, RenderStates states) const override;

    void bind() const;

   private:
    uint32_t m_VBO;
    std::size_t m_size;
};

#endif
