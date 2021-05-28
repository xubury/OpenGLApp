#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <stdint.h>
#include <Graphic/Export.hpp>
#include <Graphic/Drawable.hpp>
#include <Graphic/TextureArray.hpp>
#include <Graphic/ElementBuffer.hpp>

class GRAPHIC_API Mesh : public Drawable {
   public:
    Mesh(int type, const std::vector<Vertex> &vertices,
         const std::vector<uint32_t> &indices, const TextureArray &textures);

    void draw(RenderTarget &target, RenderStates states) const override;

    const Vertex *getVertex() const;

    std::size_t size() const;

   private:
    std::vector<Vertex> m_vertices;

    std::vector<uint32_t> m_indices;

    TextureArray m_textures;

    ElementBuffer m_buffer;
};

#endif
