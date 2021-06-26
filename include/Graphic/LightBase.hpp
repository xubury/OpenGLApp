#ifndef LIGHT_BASE_HPP
#define LIGHT_BASE_HPP

#include "Core/Export.hpp"
#include "Core/Transform.hpp"

#include <glm/glm.hpp>

namespace te {

class TE_API LightBase {
   public:
    enum Type { DIRECTIONAL_LIGHT, POINT_LIGHT };

    LightBase(Type type);

    virtual ~LightBase() = default;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

   private:
    Type m_type;
};

}  // namespace te

#endif /* LIGHT_BASE_HPP */
