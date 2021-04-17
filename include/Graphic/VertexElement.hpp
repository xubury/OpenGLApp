#ifndef VERTEX_ELEMENT_HPP
#define VERTEX_ELEMENT_HPP

#include <stdint.h>
#include <vector>

#include <Graphic/Drawable.hpp>

class Vertex;

class VertexElement : public Drawable {
   public:
    VertexElement();

    ~VertexElement();

    VertexElement(const VertexElement &) = delete;

    VertexElement &operator=(const VertexElement &) = delete;

    std::size_t size() const;

    bool empty() const;

    bool create(const Vertex *vertices, std::size_t vertexSize,
                uint32_t *indices, std::size_t indexSize);

    void update(const Vertex *vertices, std::size_t vertexSize,
                uint32_t *indices, std::size_t indexSize);

    void draw(RenderTarget &target, RenderStates states) const override;

    static void bind(const VertexElement *vertexElement);

   private:
    uint32_t m_VBO;
    uint32_t m_EBO;
    std::size_t m_size;
};

#endif
