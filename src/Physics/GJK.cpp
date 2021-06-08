#include "Physics/GJK.hpp"
#include "Core/Math.hpp"

namespace te {

Simplex::Simplex() : m_points(), m_size(0) {}

Simplex& Simplex::operator=(std::initializer_list<glm::vec3> list) {
    for (auto v = list.begin(); v != list.end(); v++) {
        m_points[std::distance(list.begin(), v)] = *v;
    }
    m_size = list.size();

    return *this;
}

void Simplex::pushFront(const glm::vec3& point) {
    m_points = {point, m_points[0], m_points[1], m_points[2]};
    m_size = std::min(m_size + 1u, 4u);
}

glm::vec3& Simplex::operator[](uint32_t i) { return m_points[i]; }

uint32_t Simplex::size() const { return m_size; }

std::array<glm::vec3, 4>::const_iterator Simplex::begin() const {
    return m_points.begin();
}

std::array<glm::vec3, 4>::const_iterator Simplex::end() const {
    return m_points.end() - (4 - m_size);
}

static bool line(Simplex& points, glm::vec3& direction) {
    const glm::vec3 a = points[0];
    const glm::vec3 b = points[1];
    const glm::vec3 ab = b - a;
    const glm::vec3 ao = -a;
    if (sameDirection(ab, ao)) {
        direction = glm::cross(glm::cross(ab, ao), ab);
    } else {
        points = {a};
        direction = ao;
    }
    return false;
}

static bool triangle(Simplex& points, glm::vec3& direction) {
    const glm::vec3 a = points[0];
    const glm::vec3 b = points[1];
    const glm::vec3 c = points[2];

    const glm::vec3 ab = b - a;
    const glm::vec3 ac = c - a;
    const glm::vec3 ao = -a;
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
    const glm::vec3 a = points[0];
    const glm::vec3 b = points[1];
    const glm::vec3 c = points[2];
    const glm::vec3 d = points[3];

    const glm::vec3 ab = b - a;
    const glm::vec3 ac = c - a;
    const glm::vec3 ad = d - a;
    const glm::vec3 ao = -a;

    const ::glm::vec3 abc = cross(ab, ac);
    const ::glm::vec3 acd = cross(ac, ad);
    const ::glm::vec3 adb = cross(ad, ab);

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

static glm::vec3 findSupport(const Collider& colliderA,
                             const Collider& colliderB,
                             const glm::vec3& direction) {
    return colliderA.findFurthestPoint(direction) -
           colliderB.findFurthestPoint(-direction);
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

bool gjk(const Collider& colliderA, const Collider& colliderB,
         std::size_t maxIterartion) {
    // TODO: initial direction?
    glm::vec3 support = findSupport(colliderA, colliderB, glm::vec3(1.0));
    Simplex points;
    points.pushFront(support);
    glm::vec3 direction = -support;
    std::size_t iteration = 0;
    while (iteration++ < maxIterartion) {
        support = findSupport(colliderA, colliderB, direction);
        if (!sameDirection(support, direction)) {
            break;
        }
        points.pushFront(support);
        if (nextSimplex(points, direction)) {
            return true;
        }
    }
    return false;
}

}  // namespace te
