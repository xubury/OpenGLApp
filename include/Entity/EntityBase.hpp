#ifndef ENTITY_BASE_HPP
#define ENTITY_BASE_HPP

#include "ECS/Entity.hpp"
#include "Core/Transform.hpp"

namespace te {

class EntityBase : public Entity<EntityBase>,
                   virtual public Transformable,
                   public Drawable {
   public:
    EntityBase(const EntityBase &) = delete;

    EntityBase &operator=(const EntityBase &) = delete;

    virtual ~EntityBase() = default;

    EntityBase(EntityManager<EntityBase> *manager, uint32_t id);

    std::string getName() const;

    void setName(const std::string &name);

    void draw(const Shader &shader, const glm::mat4 &transform) const;

   private:
    std::string m_name;
};

}  // namespace te

#endif
