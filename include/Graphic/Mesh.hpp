#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <stdint.h>
#include <Graphic/Drawable.hpp>
#include <Graphic/TextureArray.hpp>
#include <Graphic/ElementBuffer.hpp>

class Mesh : public Drawable {
   public:
    Mesh(const std::vector<Vertex> &vertices,
         const std::vector<uint32_t> &indices, const TextureArray &textures);

    void draw(RenderTarget &target, RenderStates states) const override;

   private:
    std::vector<Vertex> m_vertices;

    std::vector<uint32_t> m_indices;

    TextureArray m_textures;

    std::shared_ptr<ElementBuffer> m_buffer;
};

#endif
