#ifndef LIGHT_BASE_HPP
#define LIGHT_BASE_HPP

#include "Core/Export.hpp"

#include <glm/glm.hpp>

namespace te {

class TE_API LightBase {
   public:
    enum LightType { DIRECTIONAL, POINT };
    virtual ~LightBase() = default;

    virtual glm::mat4 getLightSpaceMatrix() const = 0;
    virtual glm::vec3 getDirection() const = 0;
    virtual glm::vec3 getPosition() const = 0;
    glm::vec3 amibent;
    glm::vec3 diffuse;
    glm::vec3 specular;
    uint32_t shadowMap;
};

}  // namespace te

#endif /* LIGHT_BASE_HPP */
