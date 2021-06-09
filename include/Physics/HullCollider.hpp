#ifndef HULL_COLLIDER_HPP
#define HULL_COLLIDER_HPP

#include "Physics/Collider.hpp"

namespace te {

class HullCollider : public Collider {
   public:
    HullCollider();

    HullCollider(const std::vector<glm::vec3>& vertices);

    void addVertex(const glm::vec3& vertex);

    void clear();

    glm::vec3 findFurthestPoint(const glm::vec3& direction) const override;

   private:
    std::vector<glm::vec3> m_vertices;
};

inline void MakeCubeCollider(HullCollider& collider, float width, float height,
                             float length) {
    collider.clear();
    collider.addVertex(glm::vec3(-width, -height, -length));  // 0
    collider.addVertex(glm::vec3(-width, height, -length));   // 1
    collider.addVertex(glm::vec3(width, height, -length));    // 2
    collider.addVertex(glm::vec3(width, -height, -length));   // 3

    collider.addVertex(glm::vec3(-width, -height, length));  // 7
    collider.addVertex(glm::vec3(width, -height, length));   // 6
    collider.addVertex(glm::vec3(width, height, length));    // 5
    collider.addVertex(glm::vec3(-width, height, length));   // 4
}

}  // namespace te

#endif /* HULL_COLLIDER_HPP */
