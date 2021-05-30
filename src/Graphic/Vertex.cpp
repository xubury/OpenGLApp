#include <Graphic/Vertex.hpp>
#include <Graphic/OpenGL.hpp>

void Vertex::setupAttribute() {
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, color));

    // texture coord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(offsetof(Vertex, texCoord)));

    // normal attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)(offsetof(Vertex, normal)));
}

Vertex::Vertex(const glm::vec3 &position, const glm::vec4 &color)
    : position(position), color(color) {}

Vertex::Vertex(const glm::vec3 &position, const glm::vec4 &color,
               const glm::vec2 &texCoord, const glm::vec3 &normal)
    : position(position), color(color), texCoord(texCoord), normal(normal) {}
