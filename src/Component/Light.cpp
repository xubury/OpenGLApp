#include "Component/Light.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace te {

DirectionalLight::DirectionalLight()
    : LightBase(LightBase::Type::DIRECTIONAL_LIGHT) {}

PointLight::PointLight()
    : LightBase(LightBase::POINT_LIGHT),
      constant(1.f),
      linear(0.09f),
      quadratic(0.032),
      cutOff(glm::cos(glm::radians(12.5f))),
      outerCutOff(glm::cos(glm::radians(67.5f))) {}

}  // namespace te
