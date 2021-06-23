#include "Model/Animator.hpp"
#include "Graphic/Vertex.hpp"
#include "Core/Assert.hpp"

namespace te {

Animator::Animator(Animation *animtion) : m_currentAnimation(animtion) {
    m_currentTime = 0.f;

    m_finalBonesMatrices.resize(MAX_BONES);

    for (int i = 0; i < MAX_BONES; ++i) {
        m_finalBonesMatrices[i] = glm::mat4(1.0f);
    }
}

void Animator::update(const Time &deltaTime) {
    if (m_currentAnimation) {
        m_currentTime +=
            m_currentAnimation->getTicksPerSecond() * deltaTime.count();
        m_currentTime = fmod(m_currentTime, m_currentAnimation->getDuration());
        calculateBoneTransform(m_currentAnimation->getRootNode(),
                               glm::mat4(1.0f));
    }
}

void Animator::play(Animation *animation) {
    m_currentAnimation = animation;
    m_currentTime = 0.f;
}

void Animator::calculateBoneTransform(const BoneNode &node,
                                      const glm::mat4 &parentTransform) {
    std::string nodeName = node.name;
    glm::mat4 nodeTransform = node.transform;
    Bone *bone = m_currentAnimation->findBone(nodeName);
    if (bone) {
        bone->update(m_currentTime);
        nodeTransform = bone->getLocalTransform();
    }
    glm::mat4 globalTransform = parentTransform * nodeTransform;
    auto &boneInfoMap = m_currentAnimation->getBoneInfoMap();
    if (boneInfoMap.count(nodeName)) {
        uint32_t index = boneInfoMap.at(nodeName).id;
        glm::mat4 offset = boneInfoMap.at(nodeName).offset;
        if (index < m_finalBonesMatrices.size()) {
            m_finalBonesMatrices[index] = globalTransform * offset;
        }
    }

    for (int i = 0; i < node.childrenCount; ++i) {
        calculateBoneTransform(node.children[i], globalTransform);
    }
}

}  // namespace te
