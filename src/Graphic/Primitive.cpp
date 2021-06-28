#include "Core/Assert.hpp"
#include "Graphic/Primitive.hpp"
#include "Graphic/Camera.hpp"
#include "Graphic/Renderer.hpp"

namespace te {

struct PrimitiveVertex {
    glm::vec3 position;
    glm::vec4 color;
    PrimitiveVertex() = default;
    PrimitiveVertex(const glm::vec3 &position, const glm::vec4 &color)
        : position(position), color(color) {}
};

Primitive::Primitive() {
    const char *primitiveVertex = R"(
    #version 330 core
        
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec4 aColor;

    layout (std140) uniform Camera {
        mat4 uProjectionView;
        vec3 uViewPos;
    };

    out VS_OUT {
        vec4 color;
    } vs_out;

    void main() {
        gl_Position = uProjectionView * vec4(aPos, 1.0f);
        vs_out.color = aColor;
    })";
    const char *primitiveFragment = R"(
    #version 330 core
    out vec4 fragColor;

    in VS_OUT {
        vec4 color;
    } fs_in;

    in vec4 color;
    void main() {
        fragColor = fs_in.color;
    })";

    const char *lineGeometry = R"(
    #version 330 core

    layout (lines) in;
    layout (triangle_strip, max_vertices = 4) out;

    in VS_OUT {
        vec4 color;
    } gs_in[];

    out VS_OUT {
        vec4 color;
    } gs_out;

    uniform float uThickness;

    uniform vec2 uViewportInvSize;
    void main() {
        gs_out.color = gs_in[0].color;

        float r = uThickness;

        vec4 p1 = gl_in[0].gl_Position;
        vec4 p2 = gl_in[1].gl_Position;

        vec2 dir = normalize(p2.xy - p1.xy);
        vec2 normal = vec2(dir.y, -dir.x);

        vec4 offset1, offset2;
        offset1 = vec4(normal * uViewportInvSize * (r * p1.w), 0, 0);
        offset2 = vec4(normal * uViewportInvSize * (r * p2.w), 0, 0); 

        vec4 coords[4];
        coords[0] = p1 + offset1;
        coords[1] = p1 - offset1;
        coords[2] = p2 + offset2;
        coords[3] = p2 - offset2;

        for (int i = 0; i < 4; ++i) {
            gl_Position = coords[i];
            EmitVertex();
        }
        EndPrimitive();
    })";
    m_shader = createScope<Shader>();
    m_shader->compile(primitiveVertex, primitiveFragment);
    m_shader->bind();
    m_shader->setUniformBlock(
        "Camera", Renderer::s_sceneData.cameraUBO->getBindingPoint());

    m_lineShader = createScope<Shader>();
    m_lineShader->compile(primitiveVertex, primitiveFragment, lineGeometry);
    m_lineShader->bind();
    m_lineShader->setUniformBlock(
        "Camera", Renderer::s_sceneData.cameraUBO->getBindingPoint());

    m_vertexArray = createRef<VertexArray>();

    m_vertexBuffer = createRef<VertexBuffer>(nullptr, 0);
    m_vertexBuffer->setLayout(
        {{ShaderDataType::Float3, "aPos"}, {ShaderDataType::Float4, "aColor"}});
    m_vertexArray->addVertexBuffer(m_vertexBuffer);

    m_indexBuffer = createRef<IndexBuffer>(nullptr, 0);
    m_vertexArray->setIndexBuffer(m_indexBuffer);
}

Primitive &Primitive::instance() {
    static Primitive s_instance;
    return s_instance;
}

void Primitive::drawLine(const glm::vec3 &start, const glm::vec3 &end,
                         const glm::vec4 &color, float thickness) {
    drawPath({start, end}, color, thickness);
}

void Primitive::drawPath(const std::vector<glm::vec3> &pts,
                         const glm::vec4 &color, float thickness) {
    std::size_t size = pts.size();
    std::vector<PrimitiveVertex> vertices(size);
    for (std::size_t i = 0; i < size; ++i) {
        vertices[i].position = pts[i];
        vertices[i].color = color;
    }
    m_lineShader->bind();
    m_lineShader->setFloat("uThickness", thickness);
    m_lineShader->setVec2("uViewportInvSize",
                          1.f / Renderer::s_sceneData.viewportSize);
    m_vertexBuffer->update(vertices.data(),
                           vertices.size() * sizeof(PrimitiveVertex));
    Renderer::submit(*m_lineShader, *m_vertexArray, GL_LINE_STRIP, false);
}

void Primitive::drawCircle(const glm::vec3 &center, const glm::vec3 &normal,
                           const glm::vec4 &color, float radius,
                           int fragments) {
    TE_CORE_ASSERT(fragments > 0, "Primitive::drawCircle fragments <= 0");
    glm::vec3 v3 = glm::normalize(normal);
    glm::vec3 v1(v3.z, 0, -v3.x);
    if (std::abs(v1.x) < std::numeric_limits<float>::epsilon() &&
        std::abs(v1.z) < std::numeric_limits<float>::epsilon()) {
        v1.z = 1.f;
    } else {
        v1 = glm::normalize(v1);
    }
    glm::vec3 v2 = glm::cross(v3, v1);
    std::vector<PrimitiveVertex> vertices;
    float increment = 2.0f * M_PI / fragments;
    glm::vec3 pt;
    for (int i = 0; i <= fragments; ++i) {
        float angle = i * increment;
        pt = center +
             radius * (v1 * glm::vec3(cos(angle)) + v2 * glm::vec3(sin(angle)));
        vertices.emplace_back(pt, color);
    }
    m_vertexBuffer->update(vertices.data(),
                           vertices.size() * sizeof(PrimitiveVertex));
    Renderer::submit(*m_shader, *m_vertexArray, GL_LINE_LOOP, false);
}

void Primitive::drawCircleFilled(const glm::vec3 &center,
                                 const glm::vec3 &normal,
                                 const glm::vec4 &color, float radius,
                                 int fragments) {
    TE_CORE_ASSERT(fragments > 0, "Primitive::drawCircle fragments <= 0");
    glm::vec3 v3 = glm::normalize(normal);
    glm::vec3 v1(v3.z, 0, -v3.x);
    if (std::abs(v1.x) < std::numeric_limits<float>::epsilon() &&
        std::abs(v1.z) < std::numeric_limits<float>::epsilon()) {
        v1.z = 1.f;
    } else {
        v1 = glm::normalize(v1);
    }
    glm::vec3 v2 = glm::cross(v3, v1);
    std::vector<PrimitiveVertex> vertices;
    vertices.emplace_back(center, color);
    float increment = 2.0f * M_PI / fragments;
    glm::vec3 pt;
    for (int i = 0; i <= fragments; ++i) {
        float angle = i * increment;
        pt = center +
             radius * (v1 * glm::vec3(cos(angle)) + v2 * glm::vec3(sin(angle)));
        vertices.emplace_back(pt, color);
    }
    m_vertexBuffer->update(vertices.data(),
                           vertices.size() * sizeof(PrimitiveVertex));
    Renderer::submit(*m_shader, *m_vertexArray, GL_TRIANGLE_FAN, false);
}

void Primitive::drawSphere(const glm::vec3 &center, const glm::vec4 &color,
                           float radius, int sectorCount, int stackCount) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawSphereFilled(center, color, radius, sectorCount, stackCount);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Primitive::drawSphereFilled(const glm::vec3 &center,
                                 const glm::vec4 &color, float radius,
                                 int sectorCount, int stackCount) {
    std::vector<PrimitiveVertex> vertices;
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
            vertices.emplace_back(glm::vec3(x, y, z) + center, color);
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
    m_vertexBuffer->update(vertices.data(),
                           vertices.size() * sizeof(PrimitiveVertex));
    m_indexBuffer->update(indices.data(), indices.size());
    Renderer::submit(*m_shader, *m_vertexArray, GL_TRIANGLES, true);
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
    std::vector<PrimitiveVertex> vertices(4);
    for (int i = 0; i < 4; ++i) {
        vertices[i].position = corners[i];
        vertices[i].color = color;
    }
    m_vertexBuffer->update(vertices.data(),
                           vertices.size() * sizeof(PrimitiveVertex));
    m_indexBuffer->update(indices, 6);
    Renderer::submit(*m_shader, *m_vertexArray, GL_TRIANGLES, true);
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
    PrimitiveVertex vertices[8];
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

    m_vertexBuffer->update(vertices, 8 * sizeof(PrimitiveVertex));
    m_indexBuffer->update(indices, 36);
    Renderer::submit(*m_shader, *m_vertexArray, GL_TRIANGLES, true);
}

}  // namespace te
