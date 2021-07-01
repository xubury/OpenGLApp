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

ContactManifold Collision::collide(Collider *objA, Transformable *transformA,
                                   Collider *objB, Transformable *transformB) {
    int row = objA->getType();
    int col = objB->getType();
    if (row > col) {
        std::swap(row, col);
        std::swap(objA, objB);
        std::swap(transformA, transformB);
    }
    CollisionTest func = collisionTable[row][col];
    if (func != nullptr) {
        return func(objA, transformA, objB, transformB);
    } else {
        return {};
    }
}

ContactManifold Collision::collideTerrainSphere(Collider *objA,
                                                Transformable *transformA,
                                                Collider *objB,
                                                Transformable *transformB) {
    TerrainCollider *terrain = dynamic_cast<TerrainCollider *>(objA);
    SphereCollider *sphere = dynamic_cast<SphereCollider *>(objB);

    glm::vec3 sphereWorldPos = transformB->toWorldSpace(sphere->getCenter());
    glm::vec3 sphereTerrainPos = transformA->toLocalSpace(sphereWorldPos);
    if (terrain->outOfBound(sphereTerrainPos.x, sphereTerrainPos.z)) {
        return {};
    }
    float height = terrain->height(sphereTerrainPos.x, sphereTerrainPos.z);
    glm::vec3 normal = terrain->normal(sphereTerrainPos.x, sphereTerrainPos.z);
    float depth = height + sphere->getRadius() - sphereTerrainPos.y;
    ContactManifold manifold;
    if (depth >= 0) {
        manifold.objA = objA->owner()->component<CollisionObject>().get();
        manifold.objB = objB->owner()->component<CollisionObject>().get();
        manifold.pointCount = 1;
        glm::vec3 worldNormal = terrain->owner()->toWorldVector(normal);
        worldNormal = glm::normalize(worldNormal);
        manifold.points[0].position =
            sphereWorldPos - worldNormal * sphere->getRadius();
        manifold.points[0].positionA =
            glm::vec3(sphereTerrainPos.x, height, sphereTerrainPos.z);
        manifold.points[0].positionB =
            transformB->toLocalSpace(manifold.points[0].position);
        manifold.points[0].depth = depth;
        objA->debugPoint = manifold.points[0].positionA;
        objB->debugPoint = manifold.points[0].positionB;
        manifold.normal = worldNormal;
        objA->debugNormal = -manifold.normal;
        objB->debugNormal = manifold.normal;
    }
    return manifold;
}

ContactManifold Collision::collideTerrainHull(Collider *objA,
                                              Transformable *transformA,
                                              Collider *objB,
                                              Transformable *transformB) {
    TerrainCollider *terrain = dynamic_cast<TerrainCollider *>(objA);
    HullCollider *hull = dynamic_cast<HullCollider *>(objB);
    glm::vec3 hullTerrainPos =
        transformA->toLocalSpace(hull->owner()->getPosition());

    if (terrain->outOfBound(hullTerrainPos.x, hullTerrainPos.z)) {
        return {};
    }

    float height = terrain->height(hullTerrainPos.x, hullTerrainPos.z);
    glm::vec3 normal = terrain->normal(hullTerrainPos.x, hullTerrainPos.z);
    glm::vec3 worldNormal = terrain->owner()->toWorldVector(normal);
    worldNormal = glm::normalize(worldNormal);
    glm::vec3 support = hull->findFurthestPoint(-worldNormal, *transformB);
    float depth = height - glm::dot(terrain->owner()->toLocalSpace(support),
                                    glm::vec3(0.f, 1.f, 0.f));
    ContactManifold manifold;
    if (depth >= 0) {
        manifold.objA = objA->owner()->component<CollisionObject>().get();
        manifold.objB = objB->owner()->component<CollisionObject>().get();
        manifold.pointCount = 1;
        manifold.points[0].position = support;
        manifold.points[0].positionA =
            glm::vec3(hullTerrainPos.x, height, hullTerrainPos.z);
        manifold.points[0].positionB = transformB->toLocalSpace(support);
        manifold.points[0].depth = depth;
        manifold.normal = worldNormal;
        objA->debugPoint = manifold.points[0].positionA;
        objB->debugPoint = manifold.points[0].positionB;
        objA->debugNormal = -manifold.normal;
        objB->debugNormal = manifold.normal;
    }
    return manifold;
}

ContactManifold Collision::collideSpheres(Collider *objA,
                                          Transformable *transformA,
                                          Collider *objB,
                                          Transformable *transformB) {
    SphereCollider *sphereA = dynamic_cast<SphereCollider *>(objA);
    SphereCollider *sphereB = dynamic_cast<SphereCollider *>(objB);
    glm::vec3 aWorldPos = transformA->toWorldSpace(sphereA->getCenter());
    glm::vec3 bWorldPos = transformB->toWorldSpace(sphereB->getCenter());
    float dist = glm::length(aWorldPos - bWorldPos);

    float depth = sphereA->getRadius() + sphereB->getRadius() - dist;
    ContactManifold manifold;
    if (depth >= 0) {
        manifold.objA = sphereA->owner()->component<CollisionObject>().get();
        manifold.objB = sphereB->owner()->component<CollisionObject>().get();
        manifold.pointCount = 1;
        manifold.points[0].position = (aWorldPos + bWorldPos) / 2.f;
        manifold.points[0].positionA =
            transformA->toLocalSpace(manifold.points[0].position);
        manifold.points[0].positionB =
            transformB->toLocalSpace(manifold.points[0].position);
        manifold.points[0].depth = depth;
        manifold.normal = (bWorldPos - aWorldPos) / dist;
        objA->debugPoint = manifold.points[0].positionA;
        objB->debugPoint = manifold.points[0].positionB;
        objA->debugNormal = -manifold.normal;
        objB->debugNormal = manifold.normal;
    }
    return manifold;
}

ContactManifold Collision::collideSphereHull(Collider *objA,
                                             Transformable *transformA,
                                             Collider *objB,
                                             Transformable *transformB) {
    auto [collide, simplex] = gjk(objA, transformA, objB, transformB, 32);
    if (collide) {
        return epa(simplex, objA, transformA, objB, transformB, 32);
    }
    return {};
}

ContactManifold Collision::collideHulls(Collider *objA,
                                        Transformable *transformA,
                                        Collider *objB,
                                        Transformable *transformB) {
    auto [collide, simplex] = gjk(objA, transformA, objB, transformB, 32);
    if (collide) {
        return epa(simplex, objA, transformA, objB, transformB, 32);
    }
    return {};
}

}  // namespace te
