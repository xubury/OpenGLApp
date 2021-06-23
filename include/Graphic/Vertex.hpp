#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "Core/Export.hpp"
#include <glm/glm.hpp>

namespace te {

const inline int MAX_BONE_INFLUENCE = 4;
const inline int MAX_BONES = 100;

struct TE_API Vertex {
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
    int boneIds[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];

    Vertex();

    Vertex(const glm::vec3 &position);

    Vertex(const glm::vec3 &position, const glm::vec2 &texCoord,
           const glm::vec3 &normal);

    void setVertexBoneData(int boneId, float weight);

    void setBoneDataToDefault();
};

}  // namespace te

#endif
