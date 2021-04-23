#include <Graphic/Axis.hpp>
#include <Graphic/RenderTarget.hpp>
#include <Graphic/Shader.hpp>

Arrow::Arrow() : m_cone(GL_TRIANGLE_FAN), m_line(GL_LINE_STRIP) {}

void Arrow::create(Dir dir, int size) {
    glm::vec3 vec(0);
    glm::vec3 color(0);
    color[dir] = 1;
    vec[dir] = size;
    DebugVertex vertices[2] = {{{0, 0, 0}, color}, {vec, color}};
    m_line.create(vertices, 2);

    std::vector<DebugVertex> coneVertices;
    coneVertices.push_back({vec * glm::vec3(1.2), color});
    float increment = 30.0f;
    float radius = 0.05f;
    for (float cur = 0.0f; cur <= 360; cur += increment) {
        if (dir == Arrow::DIR_X) {
            coneVertices.push_back(
                {vec + glm::vec3(0, radius * cos(glm::radians(cur)),
                                 radius * sin(glm::radians(cur))),
                 color});
        } else if (dir == Arrow::DIR_Y) {
            coneVertices.push_back(
                {vec + glm::vec3(radius * cos(glm::radians(cur)), 0,
                                 radius * sin(glm::radians(cur))),
                 color});
        } else if (dir == Arrow::DIR_Z) {
            coneVertices.push_back(
                {vec + glm::vec3(radius * cos(glm::radians(cur)),
                                 radius * sin(glm::radians(cur)), 0),
                 color});
        }
    }
    m_cone.create(coneVertices.data(), coneVertices.size());
}

void Arrow::draw(RenderTarget& target, RenderStates states) const {
    target.draw(m_cone, states);
    target.draw(m_line, states);
}

Axis::Axis() {}

void Axis::create(int xSize, int ySize, int zSize) {
    m_axes[0].create(Arrow::DIR_X, xSize);
    m_axes[1].create(Arrow::DIR_Y, ySize);
    m_axes[2].create(Arrow::DIR_Z, zSize);
}

void Axis::draw(RenderTarget& target, RenderStates states) const {
    DebugShader::instance().setDrawingMode(DebugShader::NORMAL);
    states.shader = &DebugShader::instance();
    for (int i = 0; i < 3; ++i) target.draw(m_axes[i], states);
}
