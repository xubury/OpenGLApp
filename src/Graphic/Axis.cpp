#include <Graphic/Axis.hpp>
#include <Graphic/RenderTarget.hpp>
#include <Graphic/Shader.hpp>

Arrow Axis::s_arrows[3];
bool Axis::s_initialized = false;

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

void Axis::create() {
    if (!s_initialized) {
        s_arrows[0].create(Arrow::DIR_X, 1);
        s_arrows[1].create(Arrow::DIR_Y, 1);
        s_arrows[2].create(Arrow::DIR_Z, 1);
    }
}

void Axis::draw(RenderTarget& target, RenderStates states) const {
    DebugShader::instance().setDrawingMode(DebugShader::NORMAL);
    states.shader = &DebugShader::instance();
    glClear(GL_DEPTH_BUFFER_BIT);
    for (int i = 0; i < 3; ++i) target.draw(s_arrows[i], states);
}
