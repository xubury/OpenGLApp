#include "Physics/PositionSolver.hpp"
#include "Physics/CollisionObject.hpp"
#include "Physics/Rigidbody.hpp"

namespace te {

void PositionSolver::solve(const std::vector<ContactManifold> &manifolds,
                           const Time &) {
    for (const ContactManifold &manifold : manifolds) {
        CollisionObject *objB = manifold.objB;
        glm::vec3 resolution =
            manifold.normal * manifold.points[0].depth * 0.1f;
        Rigidbody *body = dynamic_cast<Rigidbody *>(objB);
        if (body) {
            if (body->isKinematic()) {
                body->owner()->move(resolution);
            }
        }
    }
}
}  // namespace te
