#ifndef GJK_HPP
#define GJK_HPP

#include <array>
#include <glm/glm.hpp>

#include "Physics/Collider.hpp"
#include "Physics/Manifold.hpp"

namespace te {

struct Support {
    glm::vec3 position;
    glm::vec3 direction;
    Support() = default;
    Support(const glm::vec3& position, const glm::vec3& direction)
        : position(position), direction(direction) {}
};

class Simplex {
   public:
    Simplex();

    Simplex& operator=(std::initializer_list<Support> list);

    void pushFront(const Support& point);

    void pushFront(const Support&& point);

    Support& operator[](uint32_t i);

    uint32_t size() const;

    std::array<Support, 4>::const_iterator begin() const;

    std::array<Support, 4>::const_iterator end() const;

   private:
    std::array<Support, 4> m_points;

    uint32_t m_size;
};

glm::vec3 findSupport(const Collider* colliderA,
                      const Transformable* transformA,
                      const Collider* colliderB,
                      const Transformable* transformB,
                      const glm::vec3& direction);

std::pair<bool, Simplex> gjk(const Collider* colliderA,
                             const Transformable* transformA,
                             const Collider* colliderB,
                             const Transformable* transformB,
                             std::size_t maxIterartion);

ContactManifold epa(const Simplex& simplex, Collider* colliderA,
                    const Transformable* transformA, Collider* colliderB,
                    const Transformable* transformB, std::size_t maxIterartion);

}  // namespace te

#endif /* GJK_HPP */
