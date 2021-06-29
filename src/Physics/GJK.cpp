#include "Physics/GJK.hpp"
#include "Core/Math.hpp"

namespace te {

Simplex::Simplex() : m_size(0) {}

Simplex& Simplex::operator=(std::initializer_list<Support> list) {
    for (auto v = list.begin(); v != list.end(); v++) {
        m_points[std::distance(list.begin(), v)] = *v;
    }
    m_size = list.size();

    return *this;
}

void Simplex::pushFront(const Support& point) {
    m_points = {point, m_points[0], m_points[1], m_points[2]};
    m_size = std::min(m_size + 1u, 4u);
}

Support& Simplex::operator[](uint32_t i) { return m_points[i]; }

uint32_t Simplex::size() const { return m_size; }

std::array<Support, 4>::const_iterator Simplex::begin() const {
    return m_points.begin();
}

std::array<Support, 4>::const_iterator Simplex::end() const {
    return m_points.end() - (4 - m_size);
}

static bool line(Simplex& points, glm::vec3& direction) {
    const Support& a = points[0];
    const Support& b = points[1];

    const glm::vec3 ab = b.position - a.position;
    const glm::vec3 ao = -a.position;
    if (sameDirection(ab, ao)) {
        direction = glm::cross(glm::cross(ab, ao), ab);
    } else {
        points = {a};
        direction = ao;
    }
    return false;
}

static bool triangle(Simplex& points, glm::vec3& direction) {
    const Support& a = points[0];
    const Support& b = points[1];
    const Support& c = points[2];

    const glm::vec3 ab = b.position - a.position;
    const glm::vec3 ac = c.position - a.position;
    const glm::vec3 ao = -a.position;
    const glm::vec3 abc = glm::cross(ab, ac);
    if (sameDirection(glm::cross(abc, ac), ao)) {
        if (sameDirection(ac, ao)) {
            points = {a, c};
            direction = glm::cross(glm::cross(ac, ao), ac);
        } else {
            points = {a, b};
            return line(points, direction);
        }
    } else {
        if (sameDirection(glm::cross(ab, abc), ao)) {
            points = {a, b};
            return line(points, direction);
        } else {
            if (sameDirection(abc, ao)) {
                direction = abc;
            } else {
                points = {a, c, b};
                direction = -abc;
            }
        }
    }
    return false;
}

static bool tetrahedron(Simplex& points, glm::vec3& direction) {
    const Support& a = points[0];
    const Support& b = points[1];
    const Support& c = points[2];
    const Support& d = points[3];

    const glm::vec3 ab = b.position - a.position;
    const glm::vec3 ac = c.position - a.position;
    const glm::vec3 ad = d.position - a.position;
    const glm::vec3 ao = -a.position;

    const glm::vec3 abc = cross(ab, ac);
    const glm::vec3 acd = cross(ac, ad);
    const glm::vec3 adb = cross(ad, ab);

    if (sameDirection(abc, ao)) {
        points = {a, b, c};
        return triangle(points, direction);
    }
    if (sameDirection(acd, ao)) {
        points = {a, c, d};
        return triangle(points, direction);
    }
    if (sameDirection(adb, ao)) {
        points = {a, d, b};
        return triangle(points, direction);
    }

    return true;
}

static bool nextSimplex(Simplex& points, glm::vec3& direction) {
    switch (points.size()) {
        case 2:
            return line(points, direction);
        case 3:
            return triangle(points, direction);
        case 4:
            return tetrahedron(points, direction);
    }
    TE_CORE_TRACE("points size: {0}", points.size());
    TE_CORE_ASSERT(false, "nextSimplex error!");
    return false;
}

glm::vec3 findSupport(const Collider* colliderA, const Collider* colliderB,
                      const glm::vec3& direction) {
    return colliderA->findFurthestPoint(direction) -
           colliderB->findFurthestPoint(-direction);
}

std::pair<bool, Simplex> gjk(const Collider* colliderA,
                             const Collider* colliderB,
                             std::size_t maxIterartion) {
    // TODO: initial direction?
    glm::vec3 initDir(1.0, 0, 0);
    glm::vec3 support = findSupport(colliderA, colliderB, initDir);
    Simplex points;
    points.pushFront(Support(support, initDir));
    glm::vec3 direction = -support;
    std::size_t iteration = 0;
    while (iteration++ < maxIterartion) {
        support = findSupport(colliderA, colliderB, direction);
        if (!sameDirection(support, direction)) {
            break;
        }
        points.pushFront(Support(support, direction));
        if (nextSimplex(points, direction)) {
            return {true, points};
        }
    }
    return {false, {}};
}

static std::pair<std::vector<glm::vec4>, std::size_t> getFaceNormals(
    const std::vector<Support>& polytope,
    const std::vector<std::size_t>& faces) {
    std::vector<glm::vec4> normals;
    std::size_t minTriangle = 0;
    float minDistance = FLT_MAX;

    for (size_t i = 0; i < faces.size(); i += 3) {
        glm::vec3 a = polytope[faces[i]].position;
        glm::vec3 b = polytope[faces[i + 1]].position;
        glm::vec3 c = polytope[faces[i + 2]].position;

        glm::vec3 normal = glm::normalize(glm::cross(b - a, c - a));
        float distance = glm::dot(normal, a);

        if (distance < 0) {
            normal *= -1;
            distance *= -1;
        }

        normals.emplace_back(normal, distance);

        if (distance < minDistance) {
            minTriangle = i / 3;
            minDistance = distance;
        }
    }

    return {normals, minTriangle};
}

using EdgeList = std::vector<std::pair<std::size_t, std::size_t>>;

static void addUniqueEdge(EdgeList& edges,
                          const std::vector<std::size_t>& faces, std::size_t a,
                          std::size_t b) {
    auto reverse = std::find(edges.begin(), edges.end(),
                             std::make_pair(faces[b], faces[a]));
    if (reverse != edges.end()) {
        edges.erase(reverse);
    } else {
        edges.emplace_back(faces[a], faces[b]);
    }
}

ContactManifold epa(const Simplex& simplex, Collider* colliderA,
                    Collider* colliderB, std::size_t maxIterartion) {
    std::vector<Support> polytope(simplex.begin(), simplex.end());
    std::vector<std::size_t> faces = {0, 1, 2, 0, 3, 1, 0, 2, 3, 1, 3, 2};
    auto [normals, minFace] = getFaceNormals(polytope, faces);
    glm::vec3 minNormal;
    float minDist = std::numeric_limits<float>::max();
    std::size_t iteration = 0;
    while (std::abs(minDist - std::numeric_limits<float>::max()) <
           std::numeric_limits<float>::epsilon()) {
        minNormal = glm::vec3(normals[minFace]);
        minDist = normals[minFace].w;
        if (iteration++ >= maxIterartion) {
            break;
        }
        glm::vec3 support = findSupport(colliderA, colliderB, minNormal);
        float sDistance = glm::dot(minNormal, support);
        if (std::abs(sDistance - minDist) > 0.001f) {
            minDist = std::numeric_limits<float>::max();
            EdgeList uniqueEdges;
            for (std::size_t i = 0; i < normals.size(); ++i) {
                if (sameDirection(normals[i], support)) {
                    std::size_t f = i * 3;
                    addUniqueEdge(uniqueEdges, faces, f, f + 1);
                    addUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
                    addUniqueEdge(uniqueEdges, faces, f + 2, f);
                    faces[f + 2] = faces.back();
                    faces.pop_back();
                    faces[f + 1] = faces.back();
                    faces.pop_back();
                    faces[f] = faces.back();
                    faces.pop_back();

                    normals[i] = normals.back();
                    normals.pop_back();
                    --i;
                }
            }
            if (uniqueEdges.empty()) {
                break;
            }
            std::vector<std::size_t> newFaces;
            for (auto& [edge1, edge2] : uniqueEdges) {
                newFaces.push_back(edge1);
                newFaces.push_back(edge2);
                newFaces.push_back(polytope.size());
            }
            polytope.push_back(Support(support, minNormal));
            auto [newNormals, newMinFace] = getFaceNormals(polytope, newFaces);
            float newMinDist = std::numeric_limits<float>::max();
            for (std::size_t i = 0; i < normals.size(); ++i) {
                if (normals[i].w < newMinDist) {
                    newMinDist = normals[i].w;
                    minFace = i;
                }
            }
            if (newNormals[newMinFace].w < newMinDist) {
                minFace = newMinFace + normals.size();
            }
            faces.insert(faces.end(), newFaces.begin(), newFaces.end());
            normals.insert(normals.end(), newNormals.begin(), newNormals.end());
        }
    }
    if (std::abs(minDist - std::numeric_limits<float>::max()) <
        std::numeric_limits<float>::epsilon()) {
        return {};
    }

    glm::vec3 a = polytope[faces[minFace * 3]].position;
    glm::vec3 b = polytope[faces[minFace * 3 + 1]].position;
    glm::vec3 c = polytope[faces[minFace * 3 + 2]].position;
    glm::vec3 aDir = polytope[faces[minFace * 3]].direction;
    glm::vec3 bDir = polytope[faces[minFace * 3 + 1]].direction;
    glm::vec3 cDir = polytope[faces[minFace * 3 + 2]].direction;
    glm::vec4 plane = buildPlane(a, minNormal);
    float len = intersectRayPlane(glm::vec3(0), minNormal, plane);
    TE_CORE_ASSERT(len > 0);
    glm::vec3 cloestPoint = minNormal * len;
    float u, v, w;
    baryCentric(a, b, c, cloestPoint, u, v, w);
    glm::vec3 supportA = colliderA->findFurthestPoint(aDir);
    glm::vec3 supportB = colliderA->findFurthestPoint(bDir);
    glm::vec3 supportC = colliderA->findFurthestPoint(cDir);
    glm::vec3 contactPoint(supportA * u + supportB * v + supportC * w);

    ContactManifold manifold;
    manifold.objA = colliderA->owner()->component<CollisionObject>().get();
    manifold.objB = colliderB->owner()->component<CollisionObject>().get();
    manifold.normal = minNormal;
    manifold.pointCount = 1;
    manifold.points[0].depth = minDist + 0.001f;
    manifold.points[0].position = contactPoint;
    manifold.points[0].positionA = colliderA->owner()->toLocalSpace(contactPoint);
    manifold.points[0].positionB = colliderB->owner()->toLocalSpace(contactPoint);
    colliderA->debugPoint = contactPoint;
    colliderB->debugPoint = contactPoint;
    return manifold;
}

}  // namespace te
