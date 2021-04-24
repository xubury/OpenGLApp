#include <Graphic/BoundingBox.hpp>
#include <Graphic/Shader.hpp>
#include <Graphic/RenderTarget.hpp>
#include <Transform.hpp>
#include <glm/ext/matrix_transform.hpp>

static const uint32_t indices[36] = {0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1,
                                     7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4,
                                     4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3};

BoundingBox::BoundingBox()
    : m_elements(new ElementBuffer(GL_TRIANGLES)),
      m_vertices{{{0, 0, 0}, {0, 1.0f, 0}}, {{0, 0, 0}, {0, 1.0f, 0}},
                 {{0, 0, 0}, {0, 1.0f, 0}}, {{0, 0, 0}, {0, 1.0f, 0}},
                 {{0, 0, 0}, {0, 1.0f, 0}}, {{0, 0, 0}, {0, 1.0f, 0}},
                 {{0, 0, 0}, {0, 1.0f, 0}}, {{0, 0, 0}, {0, 1.0f, 0}}},
      m_min(std::numeric_limits<float>::max()),
      m_max(std::numeric_limits<float>::min()) {
    m_elements->create(m_vertices, 8, indices, 36);
}

void BoundingBox::draw(RenderTarget &target, RenderStates states) const {
    DebugShader::instance().setDrawingMode(DebugShader::WIRE_FRAME);
    states.shader = &DebugShader::instance();
    target.draw(*m_elements, states);
}

void BoundingBox::initialize(const Vertex *vertex, int cnt) {
    for (int i = 0; i < cnt; ++i) {
        m_min.x = std::min(m_min.x, vertex[i].position.x);
        m_min.y = std::min(m_min.y, vertex[i].position.y);
        m_min.z = std::min(m_min.z, vertex[i].position.z);
        m_max.x = std::max(m_max.x, vertex[i].position.x);
        m_max.y = std::max(m_max.y, vertex[i].position.y);
        m_max.z = std::max(m_max.z, vertex[i].position.z);
    }
    update(m_min.x, m_min.y, m_min.z, m_max.x, m_max.y, m_max.z);
}

void BoundingBox::update(float minX, float minY, float minZ, float maxX,
                         float maxY, float maxZ) {
    m_vertices[0].position[0] = minX;
    m_vertices[0].position[1] = minY;
    m_vertices[0].position[2] = maxZ;

    m_vertices[1].position[0] = maxX;
    m_vertices[1].position[1] = minY;
    m_vertices[1].position[2] = maxZ;

    m_vertices[2].position[0] = maxX;
    m_vertices[2].position[1] = maxY;
    m_vertices[2].position[2] = maxZ;

    m_vertices[3].position[0] = minX;
    m_vertices[3].position[1] = maxY;
    m_vertices[3].position[2] = maxZ;

    m_vertices[4].position[0] = minX;
    m_vertices[4].position[1] = minY;
    m_vertices[4].position[2] = minZ;

    m_vertices[5].position[0] = maxX;
    m_vertices[5].position[1] = minY;
    m_vertices[5].position[2] = minZ;

    m_vertices[6].position[0] = maxX;
    m_vertices[6].position[1] = maxY;
    m_vertices[6].position[2] = minZ;

    m_vertices[7].position[0] = minX;
    m_vertices[7].position[1] = maxY;
    m_vertices[7].position[2] = minZ;
    m_elements->update(m_vertices, 8);
}

void BoundingBox::update(const glm::mat4 &transform) {
    // implementation of Transforming Axis-Aligned Bounding Boxes by Jim Arvo
    // from "Graphics Gems", Academic Press, 1990
    // http://www.realtimerendering.com/resources/GraphicsGems/gems/TransBox.c
    glm::vec3 min(transform[3]);
    glm::vec3 max(transform[3]);
    float a;
    float b;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            a = transform[i][j] * m_min[i];
            b = transform[i][j] * m_max[i];
            if (a < b) {
                min[j] += a;
                max[j] += b;
            } else {
                min[j] += b;
                max[j] += a;
            }
        }
    }
    update(min.x, min.y, min.z, max.x, max.y, max.z);
}

glm::vec3 BoundingBox::getLocalBounding() const { return (m_max - m_min); }

void BoundingBoxSystem::update(EntityManager<DefaultEntity> &manager,
                               const Time &) {
    BoundingBox::Handle box;
    auto view = manager.getByComponents<BoundingBox>(box);
    auto end = view.end();
    for (auto cur = view.begin(); cur != end; ++cur) {
        box->update(cur->component<Transform>()->getTransform());
    }
}

void BoundingBoxSystem::draw(EntityManager<DefaultEntity> &manager,
                             RenderTarget &target) const {
    BoundingBox::Handle box;
    auto view = manager.getByComponents<BoundingBox>(box);
    auto end = view.end();
    for (auto cur = view.begin(); cur != end; ++cur) {
        box->draw(target);
    }
}
