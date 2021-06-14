#ifndef SHADOW_MAP_HPP
#define SHADOW_MAP_HPP

#include "ECS/ECS.hpp"
#include <glm/glm.hpp>
#include "Graphic/Camera.hpp"

namespace te {
class ShadowMap : public Component<ShadowMap, EntityBase> {
   public:
    ShadowMap(float dist);

    // compute lightSpaceMatrix from the camera's frustum
    void computeLightSpaceMatrix(const Ref<Camera>& camera);

    glm::mat4 getLightSpaceMatrix() const;

   private:
    glm::mat4 m_lightSapceMatrix;
    float m_shadowDist;
};

}  // namespace te

#endif /* SHADOW_MAP_HPP */
