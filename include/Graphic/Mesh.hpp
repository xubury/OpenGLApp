#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <stdint.h>
#include "Core/Export.hpp"
#include "Graphic/Drawable.hpp"
#include "Graphic/ModelTextures.hpp"
#include "Graphic/OpenGL.hpp"
#include "Graphic/Vertex.hpp"
#include "Graphic/VertexArray.hpp"

namespace te {

class TE_API Mesh : public Drawable {
   public:
    Mesh(GLenum type, const std::vector<Vertex> &vertices,
         const std::vector<uint32_t> &indices, const ModelTextures &textures);

    const Vertex *getVertex() const;

    std::size_t size() const;

    void draw(const Ref<Shader> &shader, const glm::mat4 &transform) const override;

   private:
    std::vector<Vertex> m_vertices;

    std::vector<uint32_t> m_indices;

    ModelTextures m_textures;

    Ref<VertexArray> m_vertexArray;
};

}  // namespace te

#endif
