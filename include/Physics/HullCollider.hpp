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

    const std::vector<glm::vec3>& getVertices() const { return m_vertices; }

   private:
    std::vector<glm::vec3> m_vertices;
};

inline void MakeCubeCollider(HullCollider& collider, float width, float height,
                             float length,
                             const glm::vec3& center = glm::vec3(0)) {
    collider.clear();
    collider.addVertex(glm::vec3(-width / 2.f, -height / 2.f, -length / 2.f) +
                       center);  // 0
    collider.addVertex(glm::vec3(-width / 2.f, height / 2.f, -length / 2.f) +
                       center);  // 1
    collider.addVertex(glm::vec3(width / 2.f, height / 2.f, -length / 2.f) +
                       center);  // 2
    collider.addVertex(glm::vec3(width / 2.f, -height / 2.f, -length / 2.f) +
                       center);  // 3

    collider.addVertex(glm::vec3(-width / 2.f, -height / 2.f, length / 2.f) +
                       center);  // 7
    collider.addVertex(glm::vec3(width / 2.f, -height / 2.f, length / 2.f) +
                       center);  // 6
    collider.addVertex(glm::vec3(width / 2.f, height / 2.f, length / 2.f) +
                       center);  // 5
    collider.addVertex(glm::vec3(-width / 2.f, height / 2.f, length / 2.f) +
                       center);  // 4
}

}  // namespace te

#endif /* HULL_COLLIDER_HPP */
