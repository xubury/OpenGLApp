#include "Physics/HullCollider.hpp"
#include "Physics/SphereCollider.hpp"
#include "Physics/GJK.hpp"

namespace te {

HullCollider::HullCollider() : Collider(Type::HULL_COLLIDER) {}

HullCollider::HullCollider(const std::vector<glm::vec3>& vertices)
    : Collider(Type::HULL_COLLIDER), m_vertices(vertices) {}

void HullCollider::addVertex(const glm::vec3& vertice) {
    m_vertices.push_back(vertice);
}
void HullCollider::clear() { m_vertices.clear(); }

glm::vec3 HullCollider::findFurthestPoint(const glm::vec3& direction) const {
    glm::vec3 maxPoint(0);
    float maxDistance = std::numeric_limits<float>::lowest();

    // TODO: multiple furthest point?
    int cnt = 0;
    for (const glm::vec3& vertex : m_vertices) {
        glm::vec4 v(vertex, 1);

        glm::vec3 p = glm::vec3(owner()->getTransform() * v);

        float distance = glm::dot(p, direction);
        if (distance >= maxDistance) {
            if (std::abs(distance - maxDistance) <
                std::numeric_limits<float>::epsilon()) {
                maxPoint = maxPoint + p;
                ++cnt;
            } else {
                maxDistance = distance;
                maxPoint = p;
                cnt = 1;
            }
        }
    }

    if (cnt > 1) {
        maxPoint /= cnt;
    }
    return maxPoint;
}

}  // namespace te
