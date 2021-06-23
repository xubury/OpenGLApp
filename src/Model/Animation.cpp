#include "Model/Animation.hpp"
#include "Utils/AssimpHelper.hpp"
#include "Model/Model.hpp"

namespace te {

Animation::Animation(const std::string &animationPath, Model &model) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        animationPath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        TE_CORE_ERROR("Assimp Error:{0}", importer.GetErrorString());
        return;
    }
    aiAnimation *animation = scene->mAnimations[0];
    m_duration = animation->mDuration;
    m_ticksPerSecond = animation->mTicksPerSecond;
    processNode(m_rootNode, scene->mRootNode);
    setupBones(animation, model);
}

Bone *Animation::findBone(const std::string &name) {
    auto iter = std::find_if(
        m_bones.begin(), m_bones.end(),
        [&name](const Bone &bone) { return bone.getBoneName() == name; });
    if (iter == m_bones.end())
        return nullptr;
    else
        return &(*iter);
}

void Animation::setupBones(const aiAnimation *animation, Model &model) {
    uint32_t size = animation->mNumChannels;
    std::unordered_map<std::string, BoneInfo> &boneInfoMap =
        model.getBoneInfoMap();
    int boneCount = model.getBoneCounter();
    for (uint32_t i = 0; i < size; ++i) {
        aiNodeAnim *channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;
        if (boneInfoMap.count(boneName) == 0) {
            boneInfoMap[boneName].id = boneCount;
            ++boneCount;
        }
        m_bones.emplace_back(boneInfoMap[boneName].id, boneName, channel);
    }
    m_boneInfoMap = boneInfoMap;
    model.setBoneCounter(boneCount);
}

void Animation::processNode(BoneNode &dst, const aiNode *src) {
    TE_CORE_ASSERT(src, "Error reading animation file");
    dst.name = src->mName.data;
    dst.transform = AssimpHelper::covertMatrixToGLM(src->mTransformation);
    dst.childrenCount = src->mNumChildren;

    for (uint32_t i = 0; i < src->mNumChildren; ++i) {
        BoneNode data;
        processNode(data, src->mChildren[i]);
        dst.children.push_back(data);
    }
}

}  // namespace te
