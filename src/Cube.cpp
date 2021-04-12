#include <glad/glad.h>
#include <iostream>

#include "Cube.hpp"
#include "RenderTarget.hpp"
#include "Vertex.hpp"

VertexBuffer Cube::m_buffer;

Cube::Cube() {
    if (m_buffer.size()) return;

    Vertex vertices[] = {{{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 0.0f}},
                         {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {1.0f, 0.0f}},
                         {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {1.0f, 1.0f}},
                         {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {1.0f, 1.0f}},
                         {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}},
                         {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 0.0f}},
                         {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {0.0f, 0.0f}},
                         {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}},
                         {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 1.0f}},
                         {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 1.0f}},
                         {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {0.0f, 1.0f}},
                         {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {0.0f, 0.0f}},
                         {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}},
                         {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {1.0f, 1.0f}},
                         {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}},
                         {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}},
                         {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {0.0f, 0.0f}},
                         {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}},
                         {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}},
                         {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {1.0f, 1.0f}},
                         {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}},
                         {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}},
                         {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {0.0f, 0.0f}},
                         {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}},
                         {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}},
                         {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {1.0f, 1.0f}},
                         {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}},
                         {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}},
                         {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {0.0f, 0.0f}},
                         {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}},
                         {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}},
                         {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {1.0f, 1.0f}},
                         {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}},
                         {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f}},
                         {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {0.0f, 0.0f}},
                         {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {0.0f, 1.0f}}};

    m_buffer.create(vertices, 36);
}

void Cube::draw(RenderTarget &target) const { target.draw(m_buffer); }
