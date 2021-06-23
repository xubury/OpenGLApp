#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <stdint.h>
#include "Core/Export.hpp"
#include "Graphic/Drawable.hpp"
#include "Graphic/Material.hpp"
#include "Graphic/OpenGL.hpp"
#include "Graphic/Vertex.hpp"
#include "Graphic/VertexArray.hpp"

namespace te {

class TE_API Mesh : public Drawable {
   public:
    Mesh(GLenum type, const std::vector<Vertex> &vertices,
         const std::vector<uint32_t> &indices, Ref<Material> textures);

    const Vertex *getVertex() const;

    std::size_t size() const;

    void draw(const Shader &shader, const glm::mat4 &transform) const override;

   private:
    std::vector<Vertex> m_vertices;

    std::vector<uint32_t> m_indices;

    Ref<Material> m_textures;

    Ref<VertexArray> m_vertexArray;

    GLenum m_type;
};

}  // namespace te

#endif
