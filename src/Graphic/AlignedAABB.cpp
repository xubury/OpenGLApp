#include <Graphic/AlignedAABB.hpp>
#include <Graphic/Shader.hpp>
#include <Graphic/RenderTarget.hpp>
#include <glm/ext/matrix_transform.hpp>

uint32_t AlignedAABB::s_indices[36] = {0, 1, 2, 2, 3, 0, 1, 5, 6, 6, 2, 1,
                                       7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4,
                                       4, 5, 1, 1, 0, 4, 3, 2, 6, 6, 7, 3};

Vertex AlignedAABB::s_worldVertices[8] = {
    {{0, 0, 0}, {0, 1.0f, 0}, {0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 1.0f, 0}, {0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 1.0f, 0}, {0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 1.0f, 0}, {0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 1.0f, 0}, {0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 1.0f, 0}, {0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 1.0f, 0}, {0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 1.0f, 0}, {0, 0}, {0, 0, 0}}};

AlignedAABB::AlignedAABB() {
    m_elements.create(s_worldVertices, 8, s_indices, 36);
}

void AlignedAABB::draw(RenderTarget &target, RenderStates states) const {
    states.transform = glm::mat4(1.0);
    states.shader = &DebugShader::debugShader;
    target.draw(m_elements, states);
}

void AlignedAABB::calculateAABB(Vertex *vertex, int cnt) {
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();
    float maxZ = std::numeric_limits<float>::min();
    for (int i = 0; i < cnt; ++i) {
        minX = std::min(minX, vertex[i].position.x);
        minY = std::min(minY, vertex[i].position.y);
        minZ = std::min(minZ, vertex[i].position.z);
        maxX = std::max(maxX, vertex[i].position.x);
        maxY = std::max(maxY, vertex[i].position.y);
        maxZ = std::max(maxZ, vertex[i].position.z);
    }
    m_localPosition[0].x = minX;
    m_localPosition[0].y = minY;
    m_localPosition[0].z = maxZ;

    m_localPosition[1].x = maxX;
    m_localPosition[1].y = minY;
    m_localPosition[1].z = maxZ;

    m_localPosition[2].x = maxX;
    m_localPosition[2].y = maxY;
    m_localPosition[2].z = maxZ;

    m_localPosition[3].x = minX;
    m_localPosition[3].y = maxY;
    m_localPosition[3].z = maxZ;

    m_localPosition[4].x = minX;
    m_localPosition[4].y = minY;
    m_localPosition[4].z = minZ;

    m_localPosition[5].x = maxX;
    m_localPosition[5].y = minY;
    m_localPosition[5].z = minZ;

    m_localPosition[6].x = maxX;
    m_localPosition[6].y = maxY;
    m_localPosition[6].z = minZ;

    m_localPosition[7].x = minX;
    m_localPosition[7].y = maxY;
    m_localPosition[7].z = minZ;
    setBoundingBox(minX, minY, minZ, maxX, maxY, maxZ);
}

void AlignedAABB::setBoundingBox(float minX, float minY, float minZ, float maxX,
                                 float maxY, float maxZ) {
    s_worldVertices[0].position[0] = minX;
    s_worldVertices[0].position[1] = minY;
    s_worldVertices[0].position[2] = maxZ;

    s_worldVertices[1].position[0] = maxX;
    s_worldVertices[1].position[1] = minY;
    s_worldVertices[1].position[2] = maxZ;

    s_worldVertices[2].position[0] = maxX;
    s_worldVertices[2].position[1] = maxY;
    s_worldVertices[2].position[2] = maxZ;

    s_worldVertices[3].position[0] = minX;
    s_worldVertices[3].position[1] = maxY;
    s_worldVertices[3].position[2] = maxZ;

    s_worldVertices[4].position[0] = minX;
    s_worldVertices[4].position[1] = minY;
    s_worldVertices[4].position[2] = minZ;

    s_worldVertices[5].position[0] = maxX;
    s_worldVertices[5].position[1] = minY;
    s_worldVertices[5].position[2] = minZ;

    s_worldVertices[6].position[0] = maxX;
    s_worldVertices[6].position[1] = maxY;
    s_worldVertices[6].position[2] = minZ;

    s_worldVertices[7].position[0] = minX;
    s_worldVertices[7].position[1] = maxY;
    s_worldVertices[7].position[2] = minZ;
}

void AlignedAABB::updateAABB(const glm::mat4 &transform) {
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();
    float maxZ = std::numeric_limits<float>::min();
    for (int i = 0; i < 8; ++i) {
        glm::vec3 pos = transform * glm::vec4(m_localPosition[i], 1.0);
        minX = std::min(minX, pos.x);
        minY = std::min(minY, pos.y);
        minZ = std::min(minZ, pos.z);
        maxX = std::max(maxX, pos.x);
        maxY = std::max(maxY, pos.y);
        maxZ = std::max(maxZ, pos.z);
    }
    setBoundingBox(minX, minY, minZ, maxX, maxY, maxZ);
    m_elements.update(s_worldVertices, 8, s_indices, 36);
}
