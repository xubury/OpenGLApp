#ifndef BONE_HPP
#define BONE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <assimp/scene.h>

#include "Core/Export.hpp"
#include "Core/Time.hpp"

namespace te {

struct TE_API BoneInfo {
    int id;
    glm::mat4 offset;
};

struct TE_API KeyPosition {
    glm::vec3 position;
    float timeStamp;
    KeyPosition(const glm::vec3 &position, float timeStamp)
        : position(position), timeStamp(timeStamp) {}
};

struct TE_API KeyRotation {
    glm::quat orientation;
    float timeStamp;
    KeyRotation(const glm::quat &orientation, float timeStamp)
        : orientation(orientation), timeStamp(timeStamp) {}
};

struct TE_API KeyScale {
    glm::vec3 scale;
    float timeStamp;
    KeyScale(const glm::vec3 &scale, float timeStamp)
        : scale(scale), timeStamp(timeStamp) {}
};

class TE_API Bone {
   public:
    Bone(int id, const std::string &name, const aiNodeAnim *channel);

    void update(float animationTime);

    glm::mat4 getLocalTransform() const { return m_localTransform; }

    std::string getBoneName() const { return m_name; }

    int getBoneId() const { return m_id; }

    uint32_t getPositionId(float animationTime);

    uint32_t getRotationId(float animationTime);

    uint32_t getScaleId(float animationTime);

   private:
    float getScaleFactor(float lastTimestamp, float nextTimestamp,
                         float animationTime);

    glm::mat4 interpolatePosition(float animationTime);

    glm::mat4 interpolateRotation(float animationTime);

    glm::mat4 interpolateScale(float animationTime);

   private:
    std::vector<KeyPosition> m_positions;
    std::vector<KeyRotation> m_rotations;
    std::vector<KeyScale> m_scales;

    uint32_t m_nPositions;
    uint32_t m_nRotations;
    uint32_t m_nScales;

    int m_id;
    std::string m_name;
    glm::mat4 m_localTransform;
};

}  // namespace te

#endif /* BONE_HPP */
