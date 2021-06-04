#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "Graphic/Export.hpp"
#include <glm/glm.hpp>

namespace te {

struct GRAPHIC_API Vertex {
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
    static void setupAttribute();
    Vertex() = default;
    Vertex(const glm::vec3 &position);
    Vertex(const glm::vec3 &position, const glm::vec2 &texCoord,
           const glm::vec3 &normal);
};

}  // namespace te

#endif
