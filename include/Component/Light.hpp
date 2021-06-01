#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <Graphic/LightBase.hpp>
#include <ECS/ECS.hpp>

class Light : public Component<Light, EntityBase>, public LightBase {
   public:
    Light() = default;

    glm::mat4 getLightSpaceMatrix() const override;

    glm::vec3 getDirection() const override;

    glm::vec3 getPosition() const override;
};

#endif /* LIGHT_HPP */
