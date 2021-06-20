#include "Physics/Collision.hpp"
#include "Physics/GJK.hpp"
#include "Physics/SphereCollider.hpp"
#include "Physics/HullCollider.hpp"
#include "Physics/TerrainCollider.hpp"
#include "Graphic/Primitive.hpp"

namespace te {

const CollisionTest
    Collision::collisionTable[Collider::Type::COLLIDIER_TYPE_COUNT]
                             [Collider::Type::COLLIDIER_TYPE_COUNT] = {
                                 {nullptr, collideTerrainSphere,
                                  collideTerrainHull, nullptr, nullptr},
                                 {nullptr, collideSpheres, collideSphereHull,
                                  nullptr, nullptr},
                                 {nullptr, nullptr, collideHulls, nullptr,
                                  nullptr},
                                 {nullptr, nullptr, nullptr, nullptr, nullptr},
                                 {nullptr, nullptr, nullptr, nullptr, nullptr}};

ContactManifold Collision::collide(Collider *objA, Collider *objB) {
    int row = objA->getType();
    int col = objB->getType();
    if (row > col) {
        std::swap(row, col);
        std::swap(objA, objB);
    }
    CollisionTest func = collisionTable[row][col];
    if (func != nullptr) {
        return func(objA, objB);
    } else {
        return {};
    }
}

ContactManifold Collision::collideTerrainSphere(Collider *objA,
                                                Collider *objB) {
    ContactManifold manifold;
    TerrainCollider *terrain = dynamic_cast<TerrainCollider *>(objA);
    SphereCollider *sphere = dynamic_cast<SphereCollider *>(objB);

    glm::vec3 sphereTerrainPos =
        terrain->owner()->toLocalSpace(sphere->getCenterInWorld());
    if (terrain->outOfBound(sphereTerrainPos)) {
        return manifold;
    }

    float height = terrain->height(sphereTerrainPos.x, sphereTerrainPos.z);
    glm::vec3 normal = terrain->normal(sphereTerrainPos.x, sphereTerrainPos.z);
    float depth = height + sphere->getRadius() -
                  glm::dot(sphereTerrainPos, glm::vec3(0, 1, 0));
    if (depth >= 0) {
        manifold.objA = objA->owner()->component<CollisionObject>().get();
        manifold.objB = objB->owner()->component<CollisionObject>().get();
        manifold.pointCount = 1;
        glm::vec3 worldNormal = glm::mat3(glm::transpose(glm::inverse(
                                    terrain->owner()->getTransform()))) *
                                normal;
        worldNormal = glm::normalize(worldNormal);
        manifold.points[0].position =
            sphere->getCenterInWorld() - worldNormal * sphere->getRadius();
        manifold.points[0].depth = depth;
        manifold.normal = worldNormal;
    }
    return manifold;
}

ContactManifold Collision::collideTerrainHull(Collider *objA, Collider *objB) {
    ContactManifold manifold;
    TerrainCollider *terrain = dynamic_cast<TerrainCollider *>(objA);
    HullCollider *hull = dynamic_cast<HullCollider *>(objB);
    glm::vec3 hullTerrainPos =
        terrain->owner()->toLocalSpace(hull->owner()->getPosition());
    if (terrain->outOfBound(hullTerrainPos)) {
        return manifold;
    }
    glm::vec3 normal = terrain->normal(hullTerrainPos.x, hullTerrainPos.z);
    glm::mat3 rotateInv = glm::inverse(terrain->owner()->getTransform());
    glm::vec3 worldNormal = glm::transpose(rotateInv) * normal;
    worldNormal = glm::normalize(worldNormal);
    glm::vec3 support = hull->findFurthestPoint(-worldNormal);
    float height = terrain->height(hullTerrainPos.x, hullTerrainPos.z);
    float depth = height - glm::dot(rotateInv * support, glm::vec3(0, 1, 0));
    if (depth >= 0) {
        manifold.objA = objA->owner()->component<CollisionObject>().get();
        manifold.objB = objB->owner()->component<CollisionObject>().get();
        manifold.pointCount = 1;
        manifold.points[0].position = support;
        manifold.points[0].depth = depth;
        manifold.normal = worldNormal;
    }
    return manifold;
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
    auto [collide, simlex] = gjk(objA, objB, 32);
    if (collide) {
        return epa(simlex, objA, objB, 32);
    }
    return {};
}

ContactManifold Collision::collideHulls(Collider *objA, Collider *objB) {
    auto [collide, simlex] = gjk(objA, objB, 32);
    if (collide) {
        return epa(simlex, objA, objB, 32);
    }
    return {};
}

}  // namespace te
