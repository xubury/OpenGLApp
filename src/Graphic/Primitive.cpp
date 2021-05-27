#include <Graphic/Primitive.hpp>
#include <Graphic/VertexBuffer.hpp>
#include <Graphic/ElementBuffer.hpp>
#include <Entity/Camera.hpp>

Shader Primitive::s_shader;

Primitive::Primitive() : m_vertices() { m_vertices.initialize(); }

Primitive &Primitive::instance() {
    static Primitive s_instance;
    return s_instance;
}

void Primitive::setDrawingView(const Camera *camera) {
    s_shader.use();
    glViewport(camera->getViewportX(), camera->getViewportY(),
               camera->getViewportWidth(), camera->getViewportHeight());
    s_shader.setFloat("screenX", camera->getViewportX());
    s_shader.setFloat("screenY", camera->getViewportY());
    s_shader.setFloat("screenWidth", camera->getViewportWidth());
    s_shader.setFloat("screenHeight", camera->getViewportHeight());
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
    std::vector<DebugVertex> vertex;
    float increment;
    if (fragments == 0) {
        increment = 2.0f * M_PI * 0.001;
    } else {
        increment = 2.0f * M_PI / fragments;
    }
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
    std::vector<DebugVertex> vertex;
    float increment;
    if (fragments == 0) {
        increment = 2.0f * M_PI * 0.001;
    } else {
        increment = 2.0f * M_PI / fragments;
    }
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
