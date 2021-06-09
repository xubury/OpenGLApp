#include "Physics/Collision.hpp"
#include "Physics/GJK.hpp"
#include "Physics/SphereCollider.hpp"

namespace te {

const CollisionTest
    Collision::collisionTable[Collider::Type::COLLIDIER_TYPE_COUNT]
                             [Collider::Type::COLLIDIER_TYPE_COUNT] = {
                                 {collideSpheres, collideSphereHull, nullptr,
                                  nullptr},
                                 {nullptr, collideHulls, nullptr, nullptr},
                                 {nullptr, nullptr, nullptr, nullptr},
                                 {nullptr, nullptr, nullptr, nullptr}};

ContactManifold Collision::collide(Collider *objA, Collider *objB) {
    int row = objA->getType();
    int col = objB->getType();
    if (row > col) {
        std::swap(row, col);
    }
    CollisionTest func = collisionTable[row][col];
    if (func != nullptr) {
        return func(objA, objB);
    } else {
        return {};
    }
}

ContactManifold Collision::collideSpheres(Collider *objA, Collider *objB) {
    SphereCollider *sphereA = dynamic_cast<SphereCollider *>(objA);
    SphereCollider *sphereB = dynamic_cast<SphereCollider *>(objB);
    ContactManifold manifold;
    glm::vec3 aWorldPos = sphereA->getCenterInWorld();
    glm::vec3 bWorldPos = sphereB->getCenterInWorld();
    float dist = glm::length(aWorldPos - bWorldPos);

    float depth = sphereA->getRadius() + sphereB->getRadius() - dist;
    if (depth >= 0) {
        manifold.objA = sphereA->owner()->component<CollisionObject>().get();
        manifold.objB = sphereB->owner()->component<CollisionObject>().get();
        manifold.pointCount = 1;
        manifold.points[0].position = (aWorldPos + bWorldPos) / 2.f;
        manifold.points[0].depth = depth;
        manifold.normal = (bWorldPos - aWorldPos) / dist;
    }
    return manifold;
}

ContactManifold Collision::collideSphereHull(Collider *objA, Collider *objB) {
    auto [collide, simlex] = gjk(*objA, *objB, 32);
    if (collide) {
        return epa(simlex, *objA, *objB, 32);
    }
    return {};
}

ContactManifold Collision::collideHulls(Collider *objA, Collider *objB) {
    auto [collide, simlex] = gjk(*objA, *objB, 32);
    if (collide) {
        return epa(simlex, *objA, *objB, 32);
    }
    return {};
}

}  // namespace te
