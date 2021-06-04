#include "Core/Assert.hpp"
#include "Graphic/Primitive.hpp"
#include "Graphic/VertexBuffer.hpp"
#include "Graphic/ElementBuffer.hpp"
#include "Graphic/CameraBase.hpp"

namespace te {

Primitive::Primitive() {
    const char *primitiveVertex =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 uProjection; "
        "uniform mat4 uView;"
        "void main() {\n"
        "    gl_Position = uProjection * uView * vec4(aPos, 1.0f);\n"
        "}";

    const char *primitiveFragment =
        "#version 330 core\n"
        "out vec4 fragColor;\n"
        "uniform vec4 uColor;\n"
        "void main() {\n"
        "    fragColor = uColor;\n"
        "}";
    m_shader.compile(primitiveVertex, primitiveFragment);
}

Primitive &Primitive::instance() {
    static Primitive s_instance;
    return s_instance;
}

void Primitive::setDrawingView(const CameraBase *camera) {
    m_shader.bind();
    glViewport(camera->getViewportX(), camera->getViewportY(),
               camera->getViewportWidth(), camera->getViewportHeight());
    m_shader.setMat4("uProjection", camera->getProjection());
    m_shader.setMat4("uView", camera->getView());
}

void Primitive::drawLine(const glm::vec3 &start, const glm::vec3 &end,
                         const glm::vec4 &color, float thickness) {
    drawPath({start, end}, color, thickness);
}

void Primitive::drawPath(const std::vector<glm::vec3> &pts,
                         const glm::vec4 &color, float thickness) {
    std::size_t size = pts.size();
    std::vector<Vertex> vetices(size);
    for (std::size_t i = 0; i < size; ++i) {
        vetices[i].position = pts[i];
    }
    m_vertices.update(vetices.data(), vetices.size(), GL_LINE_LOOP,
                      GL_DYNAMIC_DRAW);
    glLineWidth(thickness);
    m_shader.setVec4("uColor", color);
    m_vertices.drawPrimitive();
    glLineWidth(1.0f);
}

void Primitive::drawCircle(const glm::vec3 &center, const glm::vec4 &color,
                           float radius, int fragments) {
    TE_CORE_ASSERT(fragments > 0, "Primitive::drawCircle fragments <= 0");
    std::vector<Vertex> vertex;
    float increment = 2.0f * M_PI / fragments;
    for (float angle = 0.f; angle < 2.0f * M_PI; angle += increment) {
        vertex.emplace_back(glm::vec3(radius * cos(angle) + center.x,
                                      radius * sin(angle) + center.y,
                                      center.z));
    }
    m_vertices.update(vertex.data(), vertex.size(), GL_LINE_LOOP,
                      GL_DYNAMIC_DRAW);
    m_shader.setVec4("uColor", color);
    m_vertices.drawPrimitive();
}

void Primitive::drawCircleFilled(const glm::vec3 &center,
                                 const glm::vec4 &color, float radius,
                                 int fragments) {
    TE_CORE_ASSERT(fragments > 0, "Primitive::drawCircle fragments <= 0");
    std::vector<Vertex> vertex;
    float increment = 2.0f * M_PI / fragments;
    vertex.emplace_back(center);
    for (float angle = 0.f; angle < 2.0f * M_PI; angle += increment) {
        vertex.emplace_back(glm::vec3(radius * cos(angle) + center.x,
                                      radius * sin(angle) + center.y,
                                      center.z));
    }
    m_vertices.update(vertex.data(), vertex.size(), GL_TRIANGLE_FAN,
                      GL_DYNAMIC_DRAW);
    m_shader.setVec4("uColor", color);
    m_vertices.drawPrimitive();
}

void Primitive::drawSphere(const glm::vec3 &center, const glm::vec4 &color,
                           float radius, int sectorCount, int stackCount) {
    std::vector<Vertex> vertices;
    float x, y, z;
    float xy;

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;
    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cos(stackAngle);
        z = radius * sin(stackAngle);
        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;
            x = xy * cos(sectorAngle);
            y = xy * sin(sectorAngle);
            vertices.emplace_back(glm::vec3(x, y, z) + center);
        }
    }
    std::vector<uint32_t> indices;
    uint32_t k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);  // beginning of current stack
        k2 = k1 + sectorCount + 1;   // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);  // k1---k2---k1+1
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);  // k1---k2---k1+1
            }
        }
    }
    m_elements.update(vertices.data(), vertices.size(), indices.data(),
                      indices.size(), GL_TRIANGLES, GL_DYNAMIC_DRAW);
    m_shader.setVec4("uColor", color);
    m_elements.drawPrimitive();
}

void Primitive::drawQuad(const std::vector<glm::vec3> &corners,
                         const glm::vec4 &color, float thickness) {
    TE_CORE_ASSERT(corners.size() == 4, "Primitive::drawQuad corners size > 4");
    drawPath(corners, color, thickness);
}

void Primitive::drawQuadFilled(const std::vector<glm::vec3> &corners,
                               const glm::vec4 &color) {
    TE_CORE_ASSERT(corners.size() == 4, "Primitive::drawQuad corners size > 4");
    const uint32_t indices[6] = {0, 1, 3, 1, 2, 3};
    std::vector<Vertex> vertices(4);
    for (int i = 0; i < 4; ++i) {
        vertices[i].position = corners[i];
    }
    m_elements.update(vertices.data(), vertices.size(), indices, 6,
                      GL_TRIANGLES, GL_DYNAMIC_DRAW);
    m_shader.setVec4("uColor", color);
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
    Vertex vertices[8];
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

    m_elements.update(vertices, 8, indices, 36, GL_TRIANGLES, GL_DYNAMIC_DRAW);
    m_shader.setVec4("uColor", color);
    m_elements.drawPrimitive();
}

}  // namespace te
