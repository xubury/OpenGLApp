#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include "Core/Export.hpp"
#include "Model/Animation.hpp"

namespace te {

class Animator {
   public:
    Animator(Animation *animation);

    void update(const Time &deltaTime);

    void play(Animation *animation);

    void calculateBoneTransform(const BoneNode &node,
                                const glm::mat4 &parentTransform);

    const std::vector<glm::mat4> &getFinalBoneMatrices() const {
        return m_finalBonesMatrices;
    }

   private:
    std::vector<glm::mat4> m_finalBonesMatrices;
    Animation *m_currentAnimation;
    float m_currentTime;
};

}  // namespace te

#endif /* ANIMATOR_HPP */
