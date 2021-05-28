#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <Graphic/Export.hpp>
#include <glm/glm.hpp>

struct GRAPHIC_API Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;
    static void setupAttribute();
};

struct GRAPHIC_API DebugVertex {
    glm::vec3 position;
    glm::vec4 color;
    static void setupAttribute();
    DebugVertex() = default;
    DebugVertex(const glm::vec3 &position, const glm::vec4 &color);
};

#endif
