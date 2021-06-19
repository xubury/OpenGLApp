#include "Entity/Terrain.hpp"
#include "Graphic/Renderer.hpp"
#include "Physics/TerrainCollider.hpp"
#include "Physics/Rigidbody.hpp"
#include "Core/Math.hpp"

namespace te {

Terrain::Terrain(EntityManager<EntityBase> *manager, uint32_t id, int gridSize,
                 int vertexCount, Ref<Material> material)
    : EntityBase(manager, id),
      m_material(material),
      m_gridSize(gridSize),
      m_vertexCount(vertexCount) {
    m_vertices.resize(vertexCount * vertexCount);
    m_normals.resize(vertexCount * vertexCount);
    std::vector<glm::vec2> texCoords(vertexCount * vertexCount);
    std::vector<uint32_t> indices(6 * (vertexCount - 1) * (vertexCount - 1));
    int vertexPointer = 0;
    float halfSize = (vertexCount - 1) * 0.5f;
    for (float i = -halfSize; i <= halfSize; ++i) {
        for (float j = -halfSize; j <= halfSize; ++j) {
            RandomGenerator random(vertexPointer);
            m_vertices[vertexPointer].x = j * gridSize;
            m_vertices[vertexPointer].y = random.rnd(0.f, 4.f);
            m_vertices[vertexPointer].z = i * gridSize;
            texCoords[vertexPointer].x = (float)j / (vertexCount - 1);
            texCoords[vertexPointer].y = (float)i / (vertexCount - 1);
            vertexPointer++;
        }
    }
    computeNormal();
    int indexPointer = 0;
    for (int gz = 0; gz < vertexCount - 1; gz++) {
        for (int gx = 0; gx < vertexCount - 1; gx++) {
            int topLeft = (gz * vertexCount) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz + 1) * vertexCount) + gx;
            int bottomRight = bottomLeft + 1;
            indices[indexPointer++] = topLeft;
            indices[indexPointer++] = bottomLeft;
            indices[indexPointer++] = topRight;
            indices[indexPointer++] = topRight;
            indices[indexPointer++] = bottomLeft;
            indices[indexPointer++] = bottomRight;
        }
    }
    m_terrain = createRef<VertexArray>();

    Ref<VertexBuffer> buffer = createRef<VertexBuffer>(
        m_vertices.data(), m_vertices.size() * sizeof(glm::vec3));
    buffer->setLayout({{ShaderDataType::Float3, "aPos"}});
    m_terrain->addVertexBuffer(buffer);

    buffer = createRef<VertexBuffer>(texCoords.data(),
                                     texCoords.size() * sizeof(glm::vec2));
    buffer->setLayout({{ShaderDataType::Float2, "aTexCoord"}});
    m_terrain->addVertexBuffer(buffer);

    buffer = createRef<VertexBuffer>(m_normals.data(),
                                     m_normals.size() * sizeof(glm::vec3));
    buffer->setLayout({{ShaderDataType::Float3, "aNormal"}});
    m_terrain->addVertexBuffer(buffer);

    m_terrain->setIndexBuffer(
        createRef<IndexBuffer>(indices.data(), indices.size()));
    add<Rigidbody>(10, false);
    add<TerrainCollider>();
}

void Terrain::draw(const Shader &shader) const {
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Renderer::submit(shader, *m_terrain, GL_TRIANGLES, true, getTransform(),
                     m_material.get());
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

float Terrain::height(const glm::vec3 &localPos) const {
    float x = localPos.x / m_gridSize + (m_vertexCount - 1) * 0.5;
    float z = localPos.z / m_gridSize + (m_vertexCount - 1) * 0.5;
    int x0 = std::floor(x);
    int z0 = std::floor(z);
    float f00 = m_vertices[z0 * m_vertexCount + x0].y;
    float f10 = m_vertices[z0 * m_vertexCount + (x0 + 1)].y;
    float f01 = m_vertices[(z0 + 1) * m_vertexCount + x0].y;
    float f11 = m_vertices[(z0 + 1) * m_vertexCount + (x0 + 1)].y;
    float xCoord = x - x0;
    float zCoord = z - z0;
    if (xCoord <= 1 - zCoord) {
        return barryCentric(glm::vec3(0, f00, 0), glm::vec3(1, f10, 0),
                            glm::vec3(0, f01, 1), glm::vec2(xCoord, zCoord));
    } else {
        return barryCentric(glm::vec3(1, f10, 0), glm::vec3(1, f11, 1),
                            glm::vec3(0, f01, 1), glm::vec2(xCoord, zCoord));
    }
}

static glm::vec3 computeFaceNormal(const glm::vec3 &a, const glm::vec3 &b,
                                   const glm::vec3 &c,
                                   const glm::vec3 &vertexNormal) {
    glm::vec3 p0 = c - b;
    glm::vec3 p1 = c - a;
    glm::vec3 faceNormal = glm::normalize(glm::cross(p0, p1));
    float dot = glm::dot(faceNormal, vertexNormal);
    return dot < 0.f ? -faceNormal : faceNormal;
}

glm::vec3 Terrain::normal(const glm::vec3 &localPos) const {
    // TODO: validate correctnes
    float x = localPos.x / m_gridSize + (m_vertexCount - 1) * 0.5;
    float z = localPos.z / m_gridSize + (m_vertexCount - 1) * 0.5;
    int x0 = std::floor(x);
    int z0 = std::floor(z);
    glm::vec3 f00 = m_normals[z0 * m_vertexCount + x0];
    glm::vec3 f11 = m_normals[(z0 + 1) * m_vertexCount + (x0 + 1)];

    glm::vec3 a = m_vertices[z0 * m_vertexCount + x0];
    glm::vec3 b = m_vertices[z0 * m_vertexCount + (x0 + 1)];
    glm::vec3 c = m_vertices[(z0 + 1) * m_vertexCount + (x0 + 1)];
    glm::vec3 d = m_vertices[(z0 + 1) * m_vertexCount + x0];
    glm::vec3 faceNormal;
    float xCoord = x - x0;
    float zCoord = z - z0;
    if (xCoord <= 1 - zCoord) {
        faceNormal = computeFaceNormal(a, b, d, f00);
    } else {
        faceNormal = computeFaceNormal(b, c, d, f11);
    }
    return glm::vec3(0, 1, 0);
}

bool Terrain::outOfBound(const glm::vec3 &localPos) const {
    float x = (localPos.x / m_gridSize + (m_vertexCount - 1) * 0.5f) /
              (m_vertexCount - 1);
    float z = (localPos.z / m_gridSize + (m_vertexCount - 1) * 0.5f) /
              (m_vertexCount - 1);
    return std::isnan(localPos.x) || std::isnan(localPos.y) ||
           std::isnan(localPos.z) || x <= 0 || z <= 0 || x >= 1 || z >= 1;
}

void Terrain::computeNormal() {
    for (int i = 1; i < m_vertexCount - 1; ++i) {
        for (int j = 1; j < m_vertexCount - 1; ++j) {
            float hL = m_vertices[i * m_vertexCount + j - 1].y;
            float hR = m_vertices[i * m_vertexCount + j + 1].y;
            float hD = m_vertices[(i - 1) * m_vertexCount + j].y;
            float hU = m_vertices[(i + 1) * m_vertexCount + j].y;
            glm::vec3 &normal = m_normals[i * m_vertexCount + j];
            normal.x = hL - hR;
            normal.z = hD - hU;
            normal.y = 2.0 * m_gridSize;
            normal = normalize(normal);
        }
    }
}

};  // namespace te
