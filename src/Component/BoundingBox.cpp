#include <Graphic/Shader.hpp>
#include <Graphic/RenderTarget.hpp>
#include <Component/Transform.hpp>
#include <Component/BoundingBox.hpp>
#include <glm/ext/matrix_transform.hpp>

BoundingBox::BoundingBox()
    : m_min(std::numeric_limits<float>::max()),
      m_max(std::numeric_limits<float>::min()) {}

void BoundingBox::initialize(const Vertex *vertex, int cnt) {
    for (int i = 0; i < cnt; ++i) {
        m_min.x = std::min(m_min.x, vertex[i].position.x);
        m_min.y = std::min(m_min.y, vertex[i].position.y);
        m_min.z = std::min(m_min.z, vertex[i].position.z);
        m_max.x = std::max(m_max.x, vertex[i].position.x);
        m_max.y = std::max(m_max.y, vertex[i].position.y);
        m_max.z = std::max(m_max.z, vertex[i].position.z);
    }
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
    m_worldMin = min;
    m_worldMax = max;
}

const glm::vec3 &BoundingBox::getLocalMin() const { return m_min; }

const glm::vec3 &BoundingBox::getLocalMax() const { return m_max; }

const glm::vec3 &BoundingBox::getWorldMin() const { return m_worldMin; }

const glm::vec3 &BoundingBox::getWorldMax() const { return m_worldMax; }

BoundingBoxSystem::BoundingBoxSystem() {}

void BoundingBoxSystem::update(EntityManager<EntityBase> &manager,
                               const Time &) {
    BoundingBox::Handle box;
    auto view = manager.getByComponents<BoundingBox>(box);
    auto end = view.end();
    for (auto cur = view.begin(); cur != end; ++cur) {
        box->update(cur->component<Transform>()->getTransform());
    }
}

