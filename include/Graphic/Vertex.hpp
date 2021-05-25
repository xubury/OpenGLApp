#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;
    static void setupAttribute();
};

struct DebugVertex {
    glm::vec3 position;
    glm::vec4 color;
    static void setupAttribute();
};

#endif
