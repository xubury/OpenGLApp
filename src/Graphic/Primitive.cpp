#include <Graphic/Primitive.hpp>
#include <Graphic/VertexBuffer.hpp>
#include <Graphic/ElementBuffer.hpp>
#include <Entity/Camera.hpp>

Shader Primitive::s_shader;

Primitive::Primitive() : m_vertices() {
    m_vertices.initialize();
    m_elements.initialize();
}

Primitive &Primitive::instance() {
    static Primitive s_instance;
    return s_instance;
}

void Primitive::setDrawingView(const Camera *camera) {
    s_shader.use();
    glViewport(camera->getViewportX(), camera->getViewportY(),
               camera->getViewportWidth(), camera->getViewportHeight());
    s_shader.setMat4("projection", camera->getProjection());
    s_shader.setMat4("view", camera->getView());
}

void Primitive::drawLine(const DebugVertex &start, const DebugVertex &end,
                         float thickness) {
    drawPath({start, end}, thickness);
}

void Primitive::drawPath(const std::vector<DebugVertex> &pts, float thickness) {
    m_vertices.update(pts.data(), pts.size(), GL_LINE_LOOP, GL_DYNAMIC_DRAW);
    glLineWidth(thickness);
    m_vertices.drawPrimitive();
    glLineWidth(1.0f);
}

void Primitive::drawCircle(const DebugVertex &center, float radius,
                           int fragments) {
    assert(fragments > 0);
    std::vector<DebugVertex> vertex;
    float increment = 2.0f * M_PI / fragments;
    for (float angle = 0.f; angle < 2.0f * M_PI; angle += increment) {
        vertex.emplace_back(glm::vec3(radius * cos(angle) + center.position.x,
                                      radius * sin(angle) + center.position.y,
                                      center.position.z),
                            center.color);
    }
    m_vertices.update(vertex.data(), vertex.size(), GL_LINE_LOOP,
                      GL_DYNAMIC_DRAW);
    m_vertices.drawPrimitive();
}

void Primitive::drawCircleFilled(const DebugVertex &center, float radius,
                                 int fragments) {
    assert(fragments > 0);
    std::vector<DebugVertex> vertex;
    float increment = 2.0f * M_PI / fragments;
    vertex.emplace_back(center);
    for (float angle = 0.f; angle < 2.0f * M_PI; angle += increment) {
        vertex.emplace_back(glm::vec3(radius * cos(angle) + center.position.x,
                                      radius * sin(angle) + center.position.y,
                                      center.position.z),
                            center.color);
    }
    m_vertices.update(vertex.data(), vertex.size(), GL_TRIANGLE_FAN,
                      GL_DYNAMIC_DRAW);
    m_vertices.drawPrimitive();
}

void Primitive::drawSphere(const DebugVertex &center, float radius,
                           int fragments) {
    assert(fragments > 0);
    std::vector<DebugVertex> vertex;
    float increment = 2.0f * M_PI / fragments;
    vertex.emplace_back(center);
    float x, y, z;
    float xy;
    for (float angle = 0.f; angle < 2.0f * M_PI; angle += increment) {
        xy = radius * cos(angle);
        z = radius * sin(angle);
        for (float sectorAngle = 0.f; sectorAngle < 2.0f * M_PI;
             sectorAngle += increment) {
            x = xy * cos(sectorAngle);
            y = xy * sin(sectorAngle);
            vertex.emplace_back(
                glm::vec3(x + center.position.x, y + center.position.y,
                          z + center.position.z),
                center.color);
        }
    }
    m_vertices.update(vertex.data(), vertex.size(), GL_TRIANGLE_FAN,
                      GL_DYNAMIC_DRAW);
    m_vertices.drawPrimitive();
}

void Primitive::drawQuad(const std::vector<DebugVertex> &corners,
                         float thickness) {
    assert(corners.size() == 4);
    drawPath(corners, thickness);
}

void Primitive::drawQuadFilled(const std::vector<DebugVertex> &corners) {
    assert(corners.size() == 4);
    const uint32_t indices[6] = {0, 1, 3, 1, 2, 3};
    m_elements.update(corners.data(), corners.size(), indices, 6, GL_TRIANGLES,
                      GL_DYNAMIC_DRAW);
    m_elements.drawPrimitive();
}

void Primitive::drawCube(const glm::vec3 &min, const glm::vec3 &max,
                         const glm::vec4 &color) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawCubeFilled(min, max, color);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Primitive::drawCubeFilled(const glm::vec3 &min, const glm::vec3 &max,
                               const glm::vec4 &color) {
    const uint32_t indices[36] = {0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1,
                                  7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4,
                                  4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3};
    DebugVertex vertices[8];
    vertices[0].position[0] = min.x;
    vertices[0].position[1] = min.y;
    vertices[0].position[2] = max.z;
    vertices[0].color = color;

    vertices[1].position[0] = max.x;
    vertices[1].position[1] = min.y;
    vertices[1].position[2] = max.z;
    vertices[1].color = color;

    vertices[2].position[0] = max.x;
    vertices[2].position[1] = max.y;
    vertices[2].position[2] = max.z;
    vertices[2].color = color;

    vertices[3].position[0] = min.x;
    vertices[3].position[1] = max.y;
    vertices[3].position[2] = max.z;
    vertices[3].color = color;

    vertices[4].position[0] = min.x;
    vertices[4].position[1] = min.y;
    vertices[4].position[2] = min.z;
    vertices[4].color = color;

    vertices[5].position[0] = max.x;
    vertices[5].position[1] = min.y;
    vertices[5].position[2] = min.z;
    vertices[5].color = color;

    vertices[6].position[0] = max.x;
    vertices[6].position[1] = max.y;
    vertices[6].position[2] = min.z;
    vertices[6].color = color;

    vertices[7].position[0] = min.x;
    vertices[7].position[1] = max.y;
    vertices[7].position[2] = min.z;
    vertices[7].color = color;

    m_elements.update(vertices, 8, indices, 36, GL_TRIANGLES, GL_DYNAMIC_DRAW);
    m_elements.drawPrimitive();
}
