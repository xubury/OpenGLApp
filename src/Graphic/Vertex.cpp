#include "Graphic/Vertex.hpp"
#include "Graphic/OpenGL.hpp"

namespace te {

Vertex::Vertex() { setBoneDataToDefault(); }

Vertex::Vertex(const glm::vec3 &position) : position(position) {
    setBoneDataToDefault();
}

Vertex::Vertex(const glm::vec3 &position, const glm::vec2 &texCoord,
               const glm::vec3 &normal)
    : position(position), texCoord(texCoord), normal(normal) {
    setBoneDataToDefault();
}

void Vertex::setVertexBoneData(int boneId, float weight) {
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
        if (boneIds[i] < 0) {
            boneIds[i] = boneId;
            weights[i] = weight;
            break;
        }
    }
}

void Vertex::setBoneDataToDefault() {
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i) {
        boneIds[i] = -1;
        weights[i] = -1;
    }
}

}  // namespace te
