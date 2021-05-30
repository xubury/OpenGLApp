#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <Graphic/Export.hpp>
#include <glm/glm.hpp>

struct GRAPHIC_API Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoord;
    glm::vec3 normal;
    static void setupAttribute();
    Vertex() = default;
    Vertex(const glm::vec3 &position, const glm::vec4 &color);
    Vertex(const glm::vec3 &position, const glm::vec4 &color,
           const glm::vec2 &texCoord, const glm::vec3 &normal);
};

#endif
