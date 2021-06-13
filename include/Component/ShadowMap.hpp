#ifndef SHADOW_MAP_HPP
#define SHADOW_MAP_HPP

#include "ECS/ECS.hpp"
#include <glm/glm.hpp>
#include "Graphic/Camera.hpp"

namespace te {
class ShadowMap : public Component<ShadowMap, EntityBase> {
   public:
    ShadowMap(float dist);

    // set shadow map perspective matrix cover the camera's frustum
    void computeCameraBound(const Ref<Camera>& camera);

    glm::mat4 getLightSpaceMatrix() const;

   private:
    void setFrustum(const glm::vec3& size);

    glm::mat4 m_lightSapceMatrix;
    float m_shadowDist;
};

}  // namespace te

#endif /* SHADOW_MAP_HPP */
