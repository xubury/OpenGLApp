#ifndef ENTITY_BASE_HPP
#define ENTITY_BASE_HPP

#include <ECS/Entity.hpp>

namespace te {

class EntityBase : public Entity<EntityBase> {
   public:
    EntityBase(const EntityBase &) = delete;

    EntityBase &operator=(const EntityBase &) = delete;

    virtual ~EntityBase() = default;

    EntityBase(EntityManager<EntityBase> *manager, uint32_t id);

    void setPosition(const glm::vec3 &position);

    glm::vec3 getPosition() const;

    void move(const glm::vec3 &offset);

    void setEulerAngle(const glm::vec3 &angle);

    glm::vec3 getEulerAngle() const;

    std::string getName() const;

    void setName(const std::string &name);

    virtual void draw(const Ref<Shader> &shader) const;

   private:
    std::string m_name;
};

}  // namespace te

#endif
