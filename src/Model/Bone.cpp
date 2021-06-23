#include "Model/Bone.hpp"
#include "Utils/AssimpHelper.hpp"
#include "Core/Assert.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace te {
    
Bone::Bone(int id, const std::string &name, const aiNodeAnim *channel)
    : m_id(id), m_name(name), m_localTransform(1.0f) {
    m_nPositions = channel->mNumPositionKeys;
    for (uint32_t i = 0; i < m_nPositions; ++i) {
        aiVector3D aiPosition = channel->mPositionKeys[i].mValue;
        float time = channel->mPositionKeys[i].mTime;
        m_positions.emplace_back(AssimpHelper::getGLMVec(aiPosition), time);
    }

    m_nRotations = channel->mNumRotationKeys;
    for (uint32_t i = 0; i < m_nRotations; ++i) {
        aiQuaternion aiRotation = channel->mRotationKeys[i].mValue;
        float time = channel->mRotationKeys[i].mTime;
        m_rotations.emplace_back(AssimpHelper::getGLMQuat(aiRotation), time);
    }

    m_nScales = channel->mNumScalingKeys;
    for (uint32_t i = 0; i < m_nScales; ++i) {
        aiVector3D scale = channel->mScalingKeys[i].mValue;
        float time = channel->mScalingKeys[i].mTime;
        m_scales.emplace_back(AssimpHelper::getGLMVec(scale), time);
    }
}

void Bone::update(float animationTime) {
    glm::mat4 translation = interpolatePosition(animationTime);
    glm::mat4 rotation = interpolateRotation(animationTime);
    glm::mat4 scale = interpolateScale(animationTime);
    m_localTransform = translation * rotation * scale;
}

uint32_t Bone::getPositionId(float animationTime) {
    for (uint32_t i = 0; i < m_nPositions - 1; ++i) {
        if (animationTime < m_positions[i + 1].timeStamp) {
            return i;
        }
    }
    TE_CORE_ASSERT(false, "error animationTime");
    return 0;
}
uint32_t Bone::getRotationId(float animationTime) {
    for (uint32_t i = 0; i < m_nRotations - 1; ++i) {
        if (animationTime < m_rotations[i + 1].timeStamp) {
            return i;
        }
    }
    TE_CORE_ASSERT(false, "error animationTime");
    return 0;
}

uint32_t Bone::getScaleId(float animationTime) {
    for (uint32_t i = 0; i < m_nScales - 1; ++i) {
        if (animationTime < m_scales[i + 1].timeStamp) {
            return i;
        }
    }
    TE_CORE_ASSERT(false, "error animationTime");
    return 0;
}

float Bone::getScaleFactor(float lastTimestamp, float nextTimestamp,
                           float animationTime) {
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimestamp;
    float framesDiff = nextTimestamp - lastTimestamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

glm::mat4 Bone::interpolatePosition(float animationTime) {
    if (1 == m_nPositions)
        return glm::translate(glm::mat4(1.0f), m_positions[0].position);

    int p0Index = getPositionId(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor =
        getScaleFactor(m_positions[p0Index].timeStamp,
                       m_positions[p1Index].timeStamp, animationTime);
    glm::vec3 finalPosition =
        glm::mix(m_positions[p0Index].position, m_positions[p1Index].position,
                 scaleFactor);
    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::interpolateRotation(float animationTime) {
    if (1 == m_nRotations) {
        glm::quat rotation = glm::normalize(m_rotations[0].orientation);
        return glm::toMat4(rotation);
    }

    int p0Index = getRotationId(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor =
        getScaleFactor(m_rotations[p0Index].timeStamp,
                       m_rotations[p1Index].timeStamp, animationTime);
    glm::quat finalRotation =
        glm::slerp(m_rotations[p0Index].orientation,
                   m_rotations[p1Index].orientation, scaleFactor);
    finalRotation = glm::normalize(finalRotation);
    return glm::toMat4(finalRotation);
}

glm::mat4 Bone::interpolateScale(float animationTime) {
    if (1 == m_nScales) return glm::scale(glm::mat4(1.0f), m_scales[0].scale);

    int p0Index = getScaleId(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor =
        getScaleFactor(m_scales[p0Index].timeStamp, m_scales[p1Index].timeStamp,
                       animationTime);
    glm::vec3 finalScale =
        glm::mix(m_scales[p0Index].scale, m_scales[p1Index].scale, scaleFactor);
    return glm::scale(glm::mat4(1.0f), finalScale);
}

}  // namespace te
