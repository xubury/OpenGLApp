#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Graphic/LightBase.hpp"
#include "Entity/EntityBase.hpp"
#include "ECS/ECS.hpp"

namespace te {

class DirectionalLight : public LightBase,
                         public Component<DirectionalLight, EntityBase> {
   public:
    DirectionalLight();
};

class PointLight : public LightBase, public Component<PointLight, EntityBase> {
   public:
    PointLight();

    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

}  // namespace te

#endif /* LIGHT_HPP */
