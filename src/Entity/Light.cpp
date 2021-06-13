#include "Entity/Light.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace te {
Light::Light(EntityManager<EntityBase> *manager, uint32_t id)
    : LightBase(Light::Type::DIRECTIONAL_LIGHT), EntityBase(manager, id) {}
}  // namespace te
