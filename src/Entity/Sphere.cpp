#include <Entity/Sphere.hpp>
#include <Component/Transform.hpp>

Sphere::Sphere(EntityManager<EntityBase> *manager, uint32_t id)
    : EntityBase(manager, id) {
    m_sphere.initialize();
    int sectorCount = 100;
    int stackCount = 100;
    std::vector<Vertex> vertices;
    float x, y, z;
    float s, t;
    float nx, ny, nz;
    float xy;
    float radius = 1.0f;

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;
    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cos(stackAngle);
        z = radius * sin(stackAngle);
        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;
            x = xy * cos(sectorAngle);
            y = xy * sin(sectorAngle);
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            nx = x / radius;
            ny = y / radius;
            nz = z / radius;
            vertices.emplace_back(glm::vec3(x, y, z), glm::vec2(s, t),
                                  glm::vec3(nx, ny, nz));
        }
    }
    std::vector<uint32_t> indices;
    uint32_t k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);  // beginning of current stack
        k2 = k1 + sectorCount + 1;   // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);  // k1---k2---k1+1
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);  // k1---k2---k1+1
            }
        }
    }
    m_sphere.update(vertices.data(), vertices.size(), indices.data(),
                    indices.size(), GL_TRIANGLES, GL_STATIC_DRAW);
}

void Sphere::draw(RenderTarget &target, RenderStates states) const {
    target.draw(m_sphere, states);
}
