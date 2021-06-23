#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Model/Bone.hpp"

namespace te {

class Model;

struct BoneNode {
    glm::mat4 transform;
    std::string name;
    int childrenCount = 0;
    std::vector<BoneNode> children;
};

class Animation {
   public:
    Animation(const std::string &animationPath, Model &model);

    Bone *findBone(const std::string &name);

    float getTicksPerSecond() const { return m_ticksPerSecond; }

    float getDuration() const { return m_duration; }

    const BoneNode &getRootNode() const { return m_rootNode; }

    const std::unordered_map<std::string, BoneInfo> &getBoneInfoMap() const {
        return m_boneInfoMap;
    }

   private:
    void setupBones(const aiAnimation *animation, Model &model);

    void processNode(BoneNode &dst, const aiNode *src);

   private:
    float m_duration;
    float m_ticksPerSecond;
    std::vector<Bone> m_bones;
    BoneNode m_rootNode;
    std::unordered_map<std::string, BoneInfo> m_boneInfoMap;
};

}  // namespace te

#endif /* ANIMATION_HPP */
