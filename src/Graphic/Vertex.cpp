#include "Graphic/Vertex.hpp"
#include "Graphic/OpenGL.hpp"

namespace te {

Vertex::Vertex(const glm::vec3 &position) : position(position) {}

Vertex::Vertex(const glm::vec3 &position, const glm::vec2 &texCoord,
               const glm::vec3 &normal)
    : position(position), texCoord(texCoord), normal(normal) {}

}  // namespace te
