#include <Graphic/Primitive.hpp>
#include <Graphic/VertexBuffer.hpp>
#include <Graphic/ElementBuffer.hpp>
#include <Entity/Camera.hpp>

PrimitiveShader &PrimitiveShader::instance() {
    static PrimitiveShader s_instance;
    return s_instance;
}

PrimitiveShader::PrimitiveShader() {
    const char *vertexCode =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec4 aColor;\n"
        "out vec4 color;\n"
        "uniform float screenX;"
        "uniform float screenY;"
        "uniform float screenWidth;"
        "uniform float screenHeight;"
        "void main() {\n"
        "    vec4 screenPos = vec4(aPos, 1.0f);\n"
        "    screenPos.x = (screenPos.x - screenX) / screenWidth * 2.f - 1.f;\n"
        "    screenPos.y = 1.f - (screenPos.y + screenY) / screenHeight * "
        "2.f;\n"
        "    gl_Position = screenPos;\n"
        "    color = aColor;\n"
        "}";

    const char *fragmentCode =
        "#version 330 core\n"
        "out vec4 fragColor;\n"
        "in vec4 color;\n"
        "void main() {\n"
        "    fragColor = color;\n"
        "}";
    compile(vertexCode, fragmentCode);
}

Primitive::Primitive() : m_vertices() { m_vertices.initialize(); }

Primitive &Primitive::instance() {
    static Primitive s_instance;
    return s_instance;
}

void Primitive::setDrawingView(const Camera *camera) {
    PrimitiveShader::instance().use();
    glViewport(camera->getViewportX(), camera->getViewportY(),
               camera->getViewportWidth(), camera->getViewportHeight());
    PrimitiveShader::instance().setFloat("screenX", camera->getViewportX());
    PrimitiveShader::instance().setFloat("screenY", camera->getViewportY());
    PrimitiveShader::instance().setFloat("screenWidth",
                                         camera->getViewportWidth());
    PrimitiveShader::instance().setFloat("screenHeight",
                                         camera->getViewportHeight());
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
