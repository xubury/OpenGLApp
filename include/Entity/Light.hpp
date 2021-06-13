#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Graphic/LightBase.hpp"
#include "ECS/ECS.hpp"

namespace te {

class Light : public LightBase, public EntityBase {
   public:
    Light(EntityManager<EntityBase> *manager, uint32_t id);
};

}  // namespace te

#endif /* LIGHT_HPP */
