#ifndef GJK_HPP
#define GJK_HPP

#include <array>
#include <glm/glm.hpp>

#include "Physics/Collider.hpp"

namespace te {

class Simplex {
   public:
    Simplex();

    Simplex& operator=(std::initializer_list<glm::vec3> list);

    void pushFront(const glm::vec3& element);

    glm::vec3& operator[](uint32_t i);

    uint32_t size() const;

    std::array<glm::vec3, 4>::const_iterator begin() const;

    std::array<glm::vec3, 4>::const_iterator end() const;

   private:
    std::array<glm::vec3, 4> m_points;

    uint32_t m_size;
};

bool gjk(const Collider& colliderA, const Collider& colliderB,
         std::size_t maxIterartion);

}  // namespace te

#endif /* GJK_HPP */