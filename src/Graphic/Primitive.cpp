#include <Graphic/VertexBuffer.hpp>
#include <Graphic/ElementBuffer.hpp>

namespace primitive {

VertexBuffer s_lineVertices(GL_LINES);

ElementBuffer s_boxElements(GL_TRIANGLES);

void initPrimitive() {
    DebugVertex vertices[2];
    s_lineVertices.create(vertices, 2);

    const uint32_t indices[36] = {0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1,
                                  7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4,
                                  4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3};

    DebugVertex boxVertices[8];
    s_boxElements.create(boxVertices, 8, indices, 36);
}

void drawLine(const glm::vec3 &start, const glm::vec3 &end) {
    DebugVertex vertices[2];
    vertices[0].position = start;
    vertices[1].position = end;
    s_lineVertices.update(vertices, 2);
    s_lineVertices.drawPrimitive();
}

void drawBox(const glm::vec3 &min, const glm::vec3 &max) {
    DebugVertex vertices[8];
    vertices[0].position[0] = min.x;
    vertices[0].position[1] = min.y;
    vertices[0].position[2] = max.z;

    vertices[1].position[0] = max.x;
    vertices[1].position[1] = min.y;
    vertices[1].position[2] = max.z;

    vertices[2].position[0] = max.x;
    vertices[2].position[1] = max.y;
    vertices[2].position[2] = max.z;

    vertices[3].position[0] = min.x;
    vertices[3].position[1] = max.y;
    vertices[3].position[2] = max.z;

    vertices[4].position[0] = min.x;
    vertices[4].position[1] = min.y;
    vertices[4].position[2] = min.z;

    vertices[5].position[0] = max.x;
    vertices[5].position[1] = min.y;
    vertices[5].position[2] = min.z;

    vertices[6].position[0] = max.x;
    vertices[6].position[1] = max.y;
    vertices[6].position[2] = min.z;

    vertices[7].position[0] = min.x;
    vertices[7].position[1] = max.y;
    vertices[7].position[2] = min.z;

    s_boxElements.update(vertices, 8);
    s_boxElements.drawPrimitive();
}

}  // namespace primitive
