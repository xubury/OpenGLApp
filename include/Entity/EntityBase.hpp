#ifndef ENTITY_BASE_HPP
#define ENTITY_BASE_HPP

#include <ECS/Entity.hpp>

class EntityBase : public Entity<EntityBase>, public Drawable {
   public:
    EntityBase(const EntityBase &) = delete;

    EntityBase &operator=(const EntityBase &) = delete;

    EntityBase(EntityManager<EntityBase> *manager, uint32_t id);

    void setPosition(const glm::vec3 &position);

    glm::vec3 getPosition() const;

    std::string getName() const;

    void setName(const std::string &name);

   private:
    std::string m_name;
};

#endif
