#include "Physics/HullCollider.hpp"
#include "Physics/SphereCollider.hpp"
#include "Physics/GJK.hpp"

namespace te {

HullCollider::HullCollider(const std::vector<glm::vec3>& vertices)
    : m_vertices(vertices) {}

void HullCollider::addVertex(const glm::vec3& vertice) {
    m_vertices.push_back(vertice);
}
void HullCollider::clear() { m_vertices.clear(); }

glm::vec3 HullCollider::findFurthestPoint(const glm::vec3& direction) const {
    glm::vec3 maxPoint(0);
    float maxDistance = std::numeric_limits<float>::lowest();

    for (const glm::vec3& vertex : m_vertices) {
        glm::vec4 v(vertex, 1);

        glm::vec3 p =
            glm::vec3(owner()->component<Transform>()->getMatrix() * v);

        float distance = glm::dot(p, direction);
        if (distance > maxDistance) {
            maxDistance = distance;
            maxPoint = p;
        }
    }

    return maxPoint;
}

ContactManifold HullCollider::testCollision(Collider& collider) {
    return collider.testCollision(*this);
}

ContactManifold HullCollider::testCollision(SphereCollider& sphere) {
    auto [collide, simplex] = gjk(*this, sphere, 32);
    if (collide) {
        return epa(simplex, *this, sphere, 32);
    }
    return {};
}

ContactManifold HullCollider::testCollision(HullCollider& hull) {
    auto [collide, simlex] = gjk(*this, hull, 32);
    if (collide) {
        return epa(simlex, *this, hull, 32);
    }
    return {};
}

}  // namespace te
