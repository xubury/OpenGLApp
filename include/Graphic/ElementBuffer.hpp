#ifndef VERTEX_ELEMENT_HPP
#define VERTEX_ELEMENT_HPP

#include <stdint.h>
#include <vector>

#include <Graphic/Vertex.hpp>
#include <Graphic/Drawable.hpp>

class ElementBuffer : public Drawable {
   public:
    ElementBuffer();

    ~ElementBuffer();

    ElementBuffer(const ElementBuffer &) = delete;

    ElementBuffer &operator=(const ElementBuffer &) = delete;

    std::size_t size() const;

    bool empty() const;

    bool create(const Vertex *vertices, std::size_t vertexSize,
                uint32_t *indices, std::size_t indexSize);

    void update(const Vertex *vertices, std::size_t vertexSize,
                const uint32_t *indices, std::size_t indexSize);

    void update(const Vertex *vertices, std::size_t vertexSize);

    void update(const uint32_t *indices, std::size_t indexSize);

    void draw(RenderTarget &target, RenderStates states) const override;

    void bind() const;

   private:
    uint32_t m_VBO;
    uint32_t m_EBO;
    std::size_t m_size;
};

#endif
