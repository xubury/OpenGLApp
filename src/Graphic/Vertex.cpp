#include <Graphic/Vertex.hpp>
#include <Graphic/OpenGL.hpp>

void Vertex::setupAttribute() {
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    // texture coord
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(offsetof(Vertex, texCoord)));

    // normal attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(offsetof(Vertex, normal)));
}

Vertex::Vertex(const glm::vec3 &position) : position(position) {}

Vertex::Vertex(const glm::vec3 &position, const glm::vec2 &texCoord,
               const glm::vec3 &normal)
    : position(position), texCoord(texCoord), normal(normal) {}
